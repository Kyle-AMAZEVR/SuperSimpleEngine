#pragma once

#include <memory>
#include "wrl/client.h"
using Microsoft::WRL::ComPtr;

struct SSAdapterInfo
{
	DXGI_ADAPTER_DESC AdapterDesc;
	IDXGIAdapter* AdapterPointer = nullptr;
};

class SSVertexBuffer;
class SSDX11IndexBuffer;
class SSDX11RenderTarget;

class SSDX11Device 
{
public:
	ID3D11Device*				GetDevice()const;
	ID3D11DeviceContext*		GetDeviceContext() const;
	IDXGISwapChain*				GetSwapChain()const;

	virtual bool				InitializeDevice(HWND windowHandle);

	virtual std::shared_ptr<class SSDX11VertexBuffer>	CreateVertexBuffer(unsigned int stride, unsigned int count, void* ptrData);
	virtual std::shared_ptr<class SSDX11IndexBuffer>	CreateIndexBuffer(std::vector<unsigned int>& inData);

	class SSDX11RenderTargetTexture2D*					CreateRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, bool bGeneratedMips = false, UINT maxMipCount = 1);
	void												ResizeRenderTargetTexture2D(SSDX11RenderTargetTexture2D* InRT2D, const UINT InWidth, const UINT InHeight);

	class SSDepthRenderTargetTexture2D*					CreateDepthRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	void												ResizeDepthRenderTargetTexture2D(SSDepthRenderTargetTexture2D* InDepthRT, const UINT InWidth, const UINT InHeight);

	class SSDX11ConstantBuffer*							CreateConstantBuffer(const UINT InBufferSize);
	void												SetConstantBufferData(SSDX11ConstantBuffer* InBuffer, BYTE* PtrData, int InSize);

	void												SetCurrentRenderTarget(SSDX11RenderTarget* InRenderTarget);

	class SSDX11PixelShader*							CompilePixelShaderFromFile(std::wstring& Path);
	

	void												ClearDefaultRenderTargetView(float color[4]);
	void												SetDefaultRenderTargetAsCurrent();
	
	virtual void										Present();
	void												ResizeRenderTarget(int inWidth, int inHeight);



protected:
	bool						CreateSwapChain(HWND windowHandle);
	bool						CreateDefaultRenderTarget(int inWidth, int inHeight);

	std::tuple<ID3D11Texture2D*, ID3D11ShaderResourceView*, std::vector<ID3D11RenderTargetView*>>
	InternalCreateRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, UINT maxMipCount = 1);

	std::tuple<ID3D11Texture2D*, ID3D11DepthStencilView*>
	InternalCreateDepthRenderTarget(const UINT InWidth, const UINT InHeight, DXGI_FORMAT eFormat);

protected:
	virtual bool				CreateDevice();	

	void						PrintCompileError(ID3DBlob* errorMessage);

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