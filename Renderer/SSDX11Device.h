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
	IDXGISwapChain*				GetSwapChain()const;

	virtual bool				InitializeDevice(HWND windowHandle) override;
	virtual bool				CreateDevice() override;
	virtual void				SetVertexShader(class SSVertexShader* vs) override;
	virtual void				SetPixelShader(class SSPixelShader* ps) override;
	virtual void				ClearCurrentRenderTarget() override;

	virtual void				Present() override;

	virtual SSVertexBuffer*		CreateVertexBuffer() override;
	virtual SSDX11IndexBuffer*	CreateIndexBuffer() override;


	virtual SSVertexBuffer*		CreateVertexBuffer(void* data, unsigned int size);

	virtual void				SetVSConstantBufferData() override;
	virtual void				SetPSConstantBufferData() override;

	virtual void				ResizeViewport(unsigned int width, unsigned int height) override;
	virtual void				ClearViewport() override;

	virtual void				SetCurrentRenderTarget(class SSViewport* viewport)override;	

	void						ClearRenderTargetView(ID3D11RenderTargetView* rtView, float color[4]);
	void						SetCurrentRenderTarget(ID3D11RenderTargetView* rtView, ID3D11DepthStencilView* depthStencilView);
	void						SetCurrentRenderTargets(ID3D11RenderTargetView** rtView, ID3D11DepthStencilView* depthStencilView);

protected:
	bool						CreateSwapChain(HWND windowHandle);	

	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<ID3D11Device> mDevice;
	ComPtr<IDXGISwapChain> mSwapChain;
	
	std::vector<SSAdapterInfo> mAdapterInfos;
	std::unique_ptr<class SSDX11Viewport> mViewport;
	ID3D11Debug* mDebug = nullptr;
};