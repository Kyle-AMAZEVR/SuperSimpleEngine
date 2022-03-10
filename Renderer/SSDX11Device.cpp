
#include "SSDX11Device.h"
#include <iostream>

#include "SSRenderTarget2D.h"
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

ID3D11DeviceContext* SSDX11Device::GetDeviceContext() const
{
	return nullptr;
}

void SSDX11Device::SetVertexShader(SSVertexShader* vs)
{
	mDeviceContext->VSSetShader(vs->GetShader(), nullptr, 0);
}

void SSDX11Device::SetPixelShader(SSPixelShader* ps)
{
	mDeviceContext->PSSetShader(ps->GetShader(), nullptr, 0);
}

void SSDX11Device::ClearCurrentRenderTarget()
{
	
}

void SSDX11Device::SetCurrentRenderTarget(SSViewport* viewport)
{
	
}


void SSDX11Device::ResizeViewport(unsigned width, unsigned height)
{
	
}

void SSDX11Device::ClearViewport()
{

}
