#pragma once

#include "SSRenderTargetBase.h"
#include "SSTexture2DBase.h"

class ENGINE_API SSRenderTargetTexture2D : public SSTexture2DBase
{
public:
	SSRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat);
	virtual void Resize(const UINT newWidth, const UINT newHeight);
	virtual void Destroy() override;	
	
	ID3D11RenderTargetView* GetRenderTargetView() { return mRenderTargetView; }

	void Clear();

protected:

	friend class SSGBuffer;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;	
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);	
};

class ENGINE_API SSDepthRenderTargetTexture2D : public SSTexture2DBase
{
public:
	SSDepthRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	virtual void Resize(const UINT newWidth, const UINT newHeight);
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView; }
	void Clear();
	virtual void Destroy() override;
protected:
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
};

class ENGINE_API SSGenericRenderTarget : public IRenderTarget
{
public:
	SSGenericRenderTarget(UINT width, UINT height, UINT count, DXGI_FORMAT eFormat = DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT eDepthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);

	SSRenderTargetTexture2D* GetOutput(UINT nIndex) ;

	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
	virtual void SetCurrentRenderTarget() override;
	virtual void Clear() override;
	virtual void Resize(UINT width, UINT height) override;

protected:
	SSRenderTargetTexture2D* mRenderTargetArray[4]{ nullptr };
	SSDepthRenderTargetTexture2D* mDepthTarget = nullptr;

	

	D3D11_VIEWPORT mViewport;

	DXGI_FORMAT mFormat;

	UINT mWidth;
	UINT mHeight;
	UINT mCount = 1;
};