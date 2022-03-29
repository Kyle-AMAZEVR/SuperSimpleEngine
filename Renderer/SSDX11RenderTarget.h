#pragma once

#include "SSRenderTargetBase.h"
#include "SSDX11Texture2D.h"

#include <wrl.h>
#include <wrl/client.h>
class SSDX11Device;
using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11RenderTargetTexture2D : public SSDX11Texture2D
{
public:	
	SSDX11RenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, bool bGeneratedMips = false, UINT maxMipCount=5);
	virtual ~SSDX11RenderTargetTexture2D();
	virtual void Resize(const UINT newWidth, const UINT newHeight);	
	virtual void SaveAsDDSFile(std::wstring filename);	
	ID3D11RenderTargetView* GetRenderTargetView(UINT mip = 0) { return mRenderTargetView[mip].Get(); }
	virtual void Clear(ID3D11DeviceContext* deviceContext);
	void Destroy();
	UINT GetMipLevels() const { return mMipLevels; }

protected:
	friend class SSGBuffer;

	ComPtr<ID3D11RenderTargetView> mRenderTargetView[10]{ nullptr };
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format, const UINT mipLevels);
	bool mGenerateMips = false;	
};

class DX11RENDERER_API SSDepthRenderTargetTexture2D : public SSDX11Texture2D
{
public:
	SSDepthRenderTargetTexture2D(const UINT width, const UINT height,  DXGI_FORMAT eFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	virtual ~SSDepthRenderTargetTexture2D();
	virtual void Resize(const UINT newWidth, const UINT newHeight);
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView.Get(); }
	virtual void Clear(ID3D11DeviceContext* deviceContext);
	virtual void Destroy();
protected:
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);

	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
};

class DX11RENDERER_API SSDX11RenderTarget : public IRenderTarget
{
public:
	SSDX11RenderTarget(UINT width, UINT height, UINT count, bool bDepthExist = true, DXGI_FORMAT eFormat = DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT eDepthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	SSDX11RenderTargetTexture2D* GetOutput(UINT nIndex);

	UINT GetCount() const { return mCount; }

	void SaveRTTexture(UINT index, std::wstring filename);

	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }

	void SetCurrentRenderTarget(SSDX11Device* device);
	void Clear(SSDX11Device* deviceContext);
	
	virtual void Resize(UINT width, UINT height) override;
	virtual void Destroy();

	bool IsDepthExist() const { return mDepthExist; }
	D3D11_VIEWPORT& GetViewport() { return mViewport; }

protected:
	friend class SSDX11Device;
	SSDX11RenderTargetTexture2D* mRenderTargetArray[4]{ nullptr };
	SSDepthRenderTargetTexture2D* mDepthTarget = nullptr;
	ID3D11RenderTargetView** mRenderTargetViews{ nullptr };
	
	D3D11_VIEWPORT mViewport;
	DXGI_FORMAT mFormat;

	UINT mWidth;
	UINT mHeight;
	UINT mCount = 1;
	bool mDepthExist = true;
};