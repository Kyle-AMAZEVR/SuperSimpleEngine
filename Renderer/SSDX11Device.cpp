#include "SSDX11Device.h"
#include <iostream>
#include "SSDX11RenderTarget.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShader.h"
#include "SSCameraManager.h"

bool SSDX11Device::CreateDevice()
{
	IDXGIFactory* pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (pFactory)
	{
		IDXGIAdapter* pAdapter = nullptr;
		HRESULT AdapterResult = S_FALSE;
		UINT i = 0;
		do
		{
			AdapterResult = pFactory->EnumAdapters(i, &pAdapter);
			if (AdapterResult == S_OK)
			{
				DXGI_ADAPTER_DESC desc;
				pAdapter->GetDesc(&desc);

				mAdapterInfos.push_back(SSAdapterInfo{ desc, pAdapter });
			}
			i++;
		} while (AdapterResult == S_OK);
	}

	D3D_FEATURE_LEVEL featureLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,

	};

	UINT length = sizeof(featureLevelArray) / sizeof(D3D_FEATURE_LEVEL);

	D3D_FEATURE_LEVEL outFeatureLevel;

	HRESULT hr = D3D11CreateDevice(mAdapterInfos[0].AdapterPointer,
		D3D_DRIVER_TYPE_UNKNOWN,
		0,
		D3D11_CREATE_DEVICE_DEBUG,
		featureLevelArray,
		length,
		D3D11_SDK_VERSION, mDevice.GetAddressOf(), &outFeatureLevel, mDeviceContext.ReleaseAndGetAddressOf());

	if (hr == 0)
	{
		std::cout << "Device created" << std::endl;
	}

	return true;
}


ID3D11Device* SSDX11Device::GetDevice() const
{
	return mDevice.Get();
}

ID3D11DeviceContext* SSDX11Device::GetDeviceContext() const
{
	return mDeviceContext.Get();
}

IDXGISwapChain* SSDX11Device::GetSwapChain() const
{
	return mSwapChain.Get();
}


bool SSDX11Device::InitializeDevice(HWND windowHandle)
{
	bool bDeviceCreated = CreateDevice();
	bool bSwapChainCreated = CreateSwapChain(windowHandle);

	return bDeviceCreated && bSwapChainCreated;
}




void SSDX11Device::SetVertexShader(SSDX11VertexShader* vs)
{
	mDeviceContext->VSSetShader(vs->GetShader(), nullptr, 0);
}

void SSDX11Device::SetPixelShader(SSDX11PixelShader* ps)
{
	mDeviceContext->PSSetShader(ps->GetShader(), nullptr, 0);
}

void SSDX11Device::ClearCurrentRenderTarget()
{
}

void SSDX11Device::SetCurrentRenderTarget(SSViewport* viewport)
{
	
}

std::shared_ptr<SSDX11VertexBuffer> SSDX11Device::CreateVertexBuffer(unsigned int stride, unsigned int count, void* ptrData)
{
	D3D11_BUFFER_DESC bufferDesc{};

	bufferDesc.BindFlags		= D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage			= D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags			= 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = static_cast<UINT>(stride * count);

	D3D11_SUBRESOURCE_DATA vertexSubresourceData{};
	vertexSubresourceData.pSysMem = ptrData;
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* PtrBuffer = nullptr;

	HR(mDevice->CreateBuffer(&bufferDesc, &vertexSubresourceData, &PtrBuffer));

	std::shared_ptr<SSDX11VertexBuffer> Result = make_shared<SSDX11VertexBuffer>(PtrBuffer, stride, count);

	return std::move(Result);
}

std::shared_ptr<SSDX11IndexBuffer> SSDX11Device::CreateIndexBuffer(std::vector<unsigned int>& inData)
{
	D3D11_BUFFER_DESC bufferDesc{};
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.MiscFlags = 0;
	bufferDesc.StructureByteStride = 0;
	bufferDesc.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * inData.size());

	D3D11_SUBRESOURCE_DATA indexSubresourceData;
	indexSubresourceData.pSysMem = &inData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

	ID3D11Buffer* ptrBuffer = nullptr;

	HR(mDevice->CreateBuffer(&bufferDesc, &indexSubresourceData, &ptrBuffer));

	std::shared_ptr<SSDX11IndexBuffer> Result = make_shared<SSDX11IndexBuffer>(ptrBuffer);

	return std::move(Result);
}

void	SSDX11Device::SetVSConstantBufferData()
{
	
}

void	SSDX11Device::SetPSConstantBufferData()
{
	
}

void SSDX11Device::ClearDefaultRenderTargetView(float color[4])
{	
	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color);
}

void SSDX11Device::SetDefaultRenderTargetAsCurrent()
{	
	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	mDeviceContext->RSSetViewports(1, &mScreenViewport);
}

void SSDX11Device::Present()
{
	HRESULT presentResult = mSwapChain->Present(0, 0);

	if (presentResult == DXGI_ERROR_DEVICE_HUNG ||
		presentResult == DXGI_ERROR_DEVICE_REMOVED ||
		presentResult == DXGI_ERROR_DEVICE_RESET)
	{
		HRESULT removedReason = mDevice->GetDeviceRemovedReason();
		wchar_t outString[100];
		size_t size = 100;
		swprintf_s(outString, size, L"Device removed! DXGI_ERROR code: 0x%X\n", removedReason);
		OutputDebugStringW(outString);
	}
}

void SSDX11Device::ResizeRenderTarget(int inWidth, int inHeight)
{
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.MaxDepth = 1;
	mScreenViewport.MinDepth = 0;
	mScreenViewport.Width = inWidth;
	mScreenViewport.Height = inHeight;
	
	CreateDefaultRenderTarget(inWidth, inHeight);
}

bool SSDX11Device::CreateSwapChain(HWND windowHandle)
{
	HR(mDevice->CheckMultisampleQualityLevels(mSwapChainFormat, mSampleCount, &m4xMSAAQuality));

	HR(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebug));

	IDXGIDevice* dxgiDevice = nullptr;
	HR(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdaptor = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdaptor));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	HR(dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2)));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		ID3D11Device1* Device1;
		ID3D11DeviceContext1* DeviceContext1;
		IDXGISwapChain1* SwapChain1;
		HR(mDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&Device1)));
		{
			(void)mDevice->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&DeviceContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = mBufferWidth;
		sd.Height = mBufferHeight;
		sd.Format = mSwapChainFormat;
		sd.SampleDesc.Count = mSampleCount;
		sd.SampleDesc.Quality = m4xMSAAQuality -1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		HR(dxgiFactory2->CreateSwapChainForHwnd(mDevice.Get(), windowHandle, &sd, nullptr, nullptr, &SwapChain1));

		{
			HR(SwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(mSwapChain.GetAddressOf())));
		}

		dxgiFactory2->Release();
	}

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdaptor);
	ReleaseCOM(dxgiFactory);

	return true;
}

bool SSDX11Device::CreateDefaultRenderTarget(int inWidth, int inHeight)
{
	check(inWidth > 0 && inHeight > 0);

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	// Resize the swap chain and recreate the render target view.
	HR(mSwapChain->ResizeBuffers(0, inWidth, inHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(mDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = inWidth;
	depthStencilDesc.Height = inHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
	depthStencilDesc.SampleDesc.Count = mSampleCount;
	depthStencilDesc.SampleDesc.Quality = m4xMSAAQuality - 1;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(mDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(mDevice->CreateDepthStencilView(mDepthStencilBuffer, 0, &mDepthStencilView));

	SetDefaultRenderTargetAsCurrent();
	
	SSCameraManager::Get().SetCurrentCameraAspectRatio(static_cast<float>(inWidth) / static_cast<float>(inHeight));

	return true;
}