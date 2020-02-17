
#include "Core.h"
#include "SSEngine12.h"
#include "SSVertexElementDeclaration.h"
#include "DXVertexTypes.h"

#include <filesystem>

void SSEngine12::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;

	mAspectRatio = static_cast<float>(mBufferWidth) / static_cast<float>(mBufferHeight);

	mViewport.TopLeftX = mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mBufferWidth);
	mViewport.Height = static_cast<float>(mBufferHeight);

	mScissorRect.left = mScissorRect.top = 0;
	mScissorRect.right = mBufferWidth;
	mScissorRect.bottom = mBufferHeight;
	
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

		HR(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator[n])));
	}

	
	HR(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator[mFrameIndex].Get(), nullptr, IID_PPV_ARGS(&mCommandList)));

	HR(mCommandList->Close());

	LoadAssets();

	for(UINT i = 0; i < FrameCount; ++i)
	{
		mFenceValues[i] = 0;		
	}
	
	HR(mDevice->CreateFence(mFenceValues[mFrameIndex], D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));	
	   	
	mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if(mFenceEvent == nullptr)
	{
		check(false);
	}

	//WaitForGPU();
}

void SSEngine12::LoadAssets()
{
	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init(0, nullptr, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;

	HR(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &signature, &error));

	HR(mDevice->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));

	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;

	check(std::filesystem::exists(L"./Shader\\SimpleShader.vs"));

	HR(D3DCompileFromFile(L"./Shader\\SimpleShader.vs", nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertexShader, &error));
	HR(D3DCompileFromFile(L"./Shader\\SimpleShader.ps", nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixelShader, nullptr));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	psoDesc.InputLayout = { SSVertexElementDeclaration::PositionColorElementDesc, 2 };
	psoDesc.pRootSignature = mRootSignature.Get();		
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
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

	VT_Position3Color4 vertices[]=
	{
		{ { 0.0f, 0.25f * mAspectRatio, 0.0f }, { 1.0f, 0.0f, 0.0f, 1.0f } },
		{ { 0.25f, -0.25f * mAspectRatio, 0.0f }, { 0.0f, 1.0f, 0.0f, 1.0f } },
		{ { -0.25f, -0.25f * mAspectRatio, 0.0f }, { 0.0f, 0.0f, 1.0f, 1.0f } }
	};

	const UINT bufferSize = sizeof(vertices);

	HR(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(bufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&mVertexBuffer)));

	UINT8* pVertexDataBegin = 0;
	CD3DX12_RANGE readRange(0, 0);
	HR(mVertexBuffer->Map(0, &readRange, reinterpret_cast<void**>(&pVertexDataBegin)));
	memcpy(pVertexDataBegin, vertices, bufferSize);
	mVertexBuffer->Unmap(0, nullptr);

	mVertexBufferView.BufferLocation = mVertexBuffer->GetGPUVirtualAddress();
	mVertexBufferView.StrideInBytes = sizeof(VT_Position3Color4);
	mVertexBufferView.SizeInBytes = bufferSize;

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
	HR(mCommandAllocator[mFrameIndex]->Reset());

	HR(mCommandList->Reset(mCommandAllocator[mFrameIndex].Get(), mPipelineState.Get()));

	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());
	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,  D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVDescriptorSize);
	mCommandList->OMSetRenderTargets(1, &rtvHandle, FALSE, nullptr);

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	mCommandList->IASetVertexBuffers(0, 1, &mVertexBufferView);
	mCommandList->DrawInstanced(3, 1, 0, 0);

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
	const UINT64 currentFenceValue = mFenceValues[mFrameIndex];
	HR(mCommandQueue->Signal(mFence.Get(), currentFenceValue));

	auto CompletedValue = mFence->GetCompletedValue();

	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (CompletedValue < mFenceValues[mFrameIndex])
	{
		HR(mFence->SetEventOnCompletion(mFenceValues[mFrameIndex], mFenceEvent));
		WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
	}	

	// Set the fence value for the next frame.
	mFenceValues[mFrameIndex] = currentFenceValue + 1;	

}


void SSEngine12::WaitForGPU()
{
	// Schedule a Signal command in the queue.
	HR(mCommandQueue->Signal(mFence.Get(), mFenceValues[mFrameIndex]));

	// Wait until the fence has been processed.
	HR(mFence->SetEventOnCompletion(mFenceValues[mFrameIndex], mFenceEvent));
	WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	mFenceValues[mFrameIndex]++;
}
