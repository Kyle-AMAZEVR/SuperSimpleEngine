#pragma once

#include "SSRenderDevice.h"
#include "SSDX11Viewport.h"

struct SSAdapterInfo
{
	DXGI_ADAPTER_DESC AdapterDesc;
	IDXGIAdapter* AdapterPointer = nullptr;
};

class SSDX11Device : public SSRenderDevice
{
public:
	bool CreateDevice();
	ID3D11DeviceContext*		GetDeviceContext() const;

	virtual void				SetVertexShader(class SSVertexShader* vs) override;
	virtual void				SetPixelShader(class SSPixelShader* ps) override;
	virtual void				ClearCurrentRenderTarget() override;

	virtual void				SetCurrentRenderTarget(class SSViewport* viewport)override;	

	virtual void				ResizeViewport(unsigned int width, unsigned int height) override;
	virtual void				ClearViewport() override;

protected:

	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;

	std::vector<SSAdapterInfo> mAdapterInfos;

	std::unique_ptr<SSDX11Viewport> mViewport;
};