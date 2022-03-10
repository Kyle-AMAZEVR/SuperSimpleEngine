#pragma once

#include "SSRenderDevice.h"
#include "SSDX11Viewport.h"

struct SSAdapterInfo
{
	DXGI_ADAPTER_DESC AdapterDesc;
	IDXGIAdapter* AdapterPointer = nullptr;
};

class DX11RENDERER_API SSDX11Device : public SSRenderDevice
{
public:
	ID3D11Device*				GetDevice()const;
	ID3D11DeviceContext*		GetDeviceContext() const;

	virtual bool				InitializeDevice(HWND windowHandle) override;
	virtual bool				CreateDevice() override;
	virtual void				SetVertexShader(class SSVertexShader* vs) override;
	virtual void				SetPixelShader(class SSPixelShader* ps) override;
	virtual void				ClearCurrentRenderTarget() override;

	virtual void				SetCurrentRenderTarget(class SSViewport* viewport)override;	

	virtual void				ResizeViewport(unsigned int width, unsigned int height) override;
	virtual void				ClearViewport() override;

protected:
	bool						CreateSwapChain(HWND windowHandle);	

	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<ID3D11Device> mDevice;
	ComPtr<IDXGISwapChain> mSwapChain;
	
	std::vector<SSAdapterInfo> mAdapterInfos;
	std::unique_ptr<class SSDX11Viewport> mViewport;
};