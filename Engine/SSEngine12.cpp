
#include "Core.h"
#include "SSEngine12.h"
#include "d3dx12.h"
#include "SSVertexElementDeclaration.h"

void SSEngine12::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;
	
	bool bDeviceCreated = CreateDevice();

	D3D12_COMMAND_QUEUE_DESC desc{};
	HR(mDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mCommandQueue)));

	CreateSwapChain();

	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	HR(mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRTVHeap)));
	mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

	for(UINT n = 0; n < FrameCount; ++n)
	{		
		HR(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));
		mDevice->CreateRenderTargetView(mRenderTargets[n].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, mRTVDescriptorSize);
	}

	HR(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));

	HR(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList)));

	HR(mCommandList->Close());
	
	HR(mDevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

	mFenceValue = 1;
	mFenceEvent = CreateEvent(nullptr, false, false, nullptr);

	if(mFenceEvent == nullptr)
	{
		check(false);
	}
}

void SSEngine12::LoadAssets()
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	HR(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));

	HR(mDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	psoDesc.InputLayout = { SSVertexElementDeclaration::PositionColorElementDesc, 2 };
	psoDesc.pRootSignature = mRootSignature.Get();	
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.StencilEnable = false;	
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.SampleDesc.Count = 1;

	HR(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState)));

}

void SSEngine12::DrawScene()
{
	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	HR(mSwapChain->Present(1, 0));

	WaitForPreviousFrame();
}

bool SSEngine12::CreateSwapChain()
{	
	DXGI_SWAP_CHAIN_DESC1 sd{};	
	
	sd.BufferCount = FrameCount;
	sd.Width = mBufferWidth;
	sd.Height = mBufferHeight;	
	sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.SampleDesc.Count = 1;

	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;
	HR(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWindowHandle, &sd, nullptr, nullptr, &tempSwapChain));
	tempSwapChain.As(&mSwapChain);	
	HR(mFactory->MakeWindowAssociation(mWindowHandle, DXGI_MWA_NO_ALT_ENTER));
	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

	return true;
}

void SSEngine12::PopulateCommandList()
{
	HR(mCommandAllocator->Reset());

	HR(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	// Record commands.
	const float clearColor[] = { 1.0f, 0.2f, 0.4f, 1.0f };

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVDescriptorSize);

	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	mCommandList->Close();
}


bool SSEngine12::CreateDevice()
{	
	ComPtr<IDXGIAdapter1> hardwareAdapter;

	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	
	HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));
	IDXGIAdapter1* adapter;

	int adapterIndex = 0;
	bool adapterFound = false;

	while(mFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapterIndex++;
			continue;
		}

		HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), &mDevice);
		if(SUCCEEDED(hr))
		{
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	return adapterFound;
}

void SSEngine12::WaitForPreviousFrame()
{
	const UINT64 fence = mFenceValue;
	mCommandQueue->Signal(mFence.Get(), fence);
	mFenceValue++;

	if(mFence->GetCompletedValue() < fence)
	{
		HR(mFence->SetEventOnCompletion(fence, mFenceEvent));

		WaitForSingleObject(mFenceEvent, INFINITE);
	}

	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();
}

