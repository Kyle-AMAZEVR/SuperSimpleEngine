#pragma once

#include <memory>
#include "SSDX11Viewport.h"

struct SSAdapterInfo
{
	DXGI_ADAPTER_DESC AdapterDesc;
	IDXGIAdapter* AdapterPointer = nullptr;
};

class SSVertexBuffer;
class SSDX11IndexBuffer;

class DX11RENDERER_API SSDX11Device 
{
public:
	ID3D11Device*				GetDevice()const;
	ID3D11DeviceContext*		GetDeviceContext() const;
	IDXGISwapChain*				GetSwapChain()const;

	virtual bool				InitializeDevice(HWND windowHandle);

	virtual void				SetVertexShader(class SSDX11VertexShader* vs);
	virtual void				SetPixelShader(class SSDX11PixelShader* ps);
	virtual void				ClearCurrentRenderTarget();	

	virtual std::shared_ptr<class SSDX11VertexBuffer>	CreateVertexBuffer(unsigned int stride, unsigned int count, void* ptrData);
	virtual std::shared_ptr<class SSDX11IndexBuffer>	CreateIndexBuffer(std::vector<unsigned int>& inData);


	virtual void				SetVSConstantBufferData();
	virtual void				SetPSConstantBufferData();

	virtual void				ResizeViewport(unsigned int width, unsigned int height);
	virtual void				ClearViewport();

	virtual void				SetCurrentRenderTarget(class SSViewport* viewport);	

	void						ClearRenderTargetView(ID3D11RenderTargetView* rtView, float color[4]);
	void						SetCurrentRenderTarget(ID3D11RenderTargetView* rtView, ID3D11DepthStencilView* depthStencilView);
	void						SetCurrentRenderTargets(ID3D11RenderTargetView** rtView, ID3D11DepthStencilView* depthStencilView);
	virtual void				Present();
protected:
	bool						CreateSwapChain(HWND windowHandle);	

protected:
	virtual bool				CreateDevice();	

	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<ID3D11Device> mDevice;
	ComPtr<IDXGISwapChain> mSwapChain;
	
	std::vector<SSAdapterInfo> mAdapterInfos;
	std::unique_ptr<class SSDX11Viewport> mViewport;
	ID3D11Debug* mDebug = nullptr;

	int mBufferWidth = 1920;
	int mBufferHeight = 1080;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
	DXGI_FORMAT mSwapChainFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	std::vector<class SSRenderCmdBase*> mRenderCommands;
};