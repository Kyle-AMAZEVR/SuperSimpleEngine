
#include "Core.h"
#include "SSEngine12.h"



void SSEngine12::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;
	
	bool bDeviceCreated = CreateDevice();

	D3D12_COMMAND_QUEUE_DESC desc{};
	HRESULT hr = mDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mCommandQueue));


}

bool SSEngine12::CreateSwapChain()
{
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

bool SSEngine12::CreateSwapChain()
{
	return true;
}


void SSEngine12::LoadPipeline()
{
	UINT dxgiFactoryFlags = 0;

#if defined(DEBUG)
	// Enable the debug layer (requires the Graphics Tools "optional feature").
	// NOTE: Enabling the debug layer after device creation will invalidate the active device.
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

	CreateDevice();

	D3D12_COMMAND_QUEUE_DESC queueDesc{};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;

	HR(mDevice->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&mCommandQueue)));

	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};

	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.BufferCount = 1;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	swapChainDesc.Width = mWidth;
	swapChainDesc.Height = mHeight;
			

	ComPtr<IDXGISwapChain1> swapChain;

	HR(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWindowHandle, &swapChainDesc, nullptr, nullptr, &swapChain));

	HR(mFactory->MakeWindowAssociation(mWindowHandle, 0));

	HR(swapChain.As(&mSwapChain));

	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

	{
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc{};

		heapDesc.NumDescriptors = FrameCount;
		heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

		HR(mDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mRTVHeap)));

		mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	}

	// Create frame resources.
	{
		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

		// Create a RTV for each frame.
		for (UINT n = 0; n < FrameCount; n++)
		{
			HR(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&m_renderTargets[n])));
			mDevice->CreateRenderTargetView(m_renderTargets[n].Get(), nullptr, rtvHandle);
			rtvHandle.Offset(1, mRTVDescriptorSize);
		}
	}

}


void SSEngine12::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;

	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);

		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// Don't select the Basic Render Driver adapter.
			// If you want a software adapter, pass in "/warp" on the command line.
			continue;
		}

		// Check to see if the adapter supports Direct3D 12, but don't create the
		// actual device yet.
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}

	*ppAdapter = adapter.Detach();
}