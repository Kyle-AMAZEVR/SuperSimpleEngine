
#include "SSDX11Device.h"
#include <iostream>

#include "SSDX11RenderTarget.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShader.h"

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



void SSDX11Device::ResizeViewport(unsigned int newWidth, unsigned int newHeight)
{
	if (mBufferWidth != newWidth || mBufferHeight != newHeight)
	{
		if (mViewport)
		{
			mViewport->Resize(this, newWidth, newHeight);
		}
	}
}

void SSDX11Device::ClearViewport()
{

}

void SSDX11Device::ClearRenderTargetView(ID3D11RenderTargetView* rtView, float color[4])
{
	mDeviceContext->ClearRenderTargetView(rtView, color);
}

void SSDX11Device::SetCurrentRenderTarget(ID3D11RenderTargetView* rtView, ID3D11DepthStencilView* depthStencilView)
{
	ID3D11RenderTargetView* rtViews[]{rtView};
	mDeviceContext->OMSetRenderTargets(1, rtViews, depthStencilView);
}

void SSDX11Device::SetCurrentRenderTargets(ID3D11RenderTargetView** rtView, ID3D11DepthStencilView* depthStencilView)
{

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


bool SSDX11Device::CreateSwapChain(HWND windowHandle)
{
	HR(mDevice->CheckMultisampleQualityLevels(mSwapChainFormat, 4, &m4xMSAAQuality));

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
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMSAAQuality - 1;
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
