
#include "Core.h"
#include "SSEngine12.h"
#include "d3dx12.h"


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

bool SSEngine12::CreateDevice()
{	
	ComPtr<IDXGIAdapter1> hardwareAdapter;
	
	HR(CreateDXGIFactory1(IID_PPV_ARGS(&mFactory)));
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

