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

	virtual std::shared_ptr<class SSDX11VertexBuffer>	CreateVertexBuffer(unsigned int stride, unsigned int count, void* ptrData);
	virtual std::shared_ptr<class SSDX11IndexBuffer>	CreateIndexBuffer(std::vector<unsigned int>& inData);

	void						ClearDefaultRenderTargetView(float color[4]);
	void						SetDefaultRenderTargetAsCurrent();
	
	virtual void				Present();
	void						ResizeRenderTarget(int inWidth, int inHeight);
protected:
	bool						CreateSwapChain(HWND windowHandle);
	bool						CreateDefaultRenderTarget(int inWidth, int inHeight);

protected:
	virtual bool				CreateDevice();	

	ComPtr<ID3D11DeviceContext> mDeviceContext;
	ComPtr<ID3D11Device>		mDevice;
	ComPtr<IDXGISwapChain>		mSwapChain;
	
	std::vector<SSAdapterInfo> mAdapterInfos;	
	ID3D11Debug* mDebug = nullptr;

	int mBufferWidth = 1920;
	int mBufferHeight = 1080;
	//
	HWND mWindowHandle;
	//
	UINT m4xMSAAQuality = 0;
	UINT mSampleCount = 4;
	
	// default screen render target releated
	ID3D11RenderTargetView* mRenderTargetView{};
	ID3D11Texture2D* mDepthStencilBuffer{};
	ID3D11DepthStencilView* mDepthStencilView{};
	D3D11_VIEWPORT mScreenViewport;

	DXGI_FORMAT mSwapChainFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;


	std::vector<class SSRenderCmdBase*> mRenderCommands;
};