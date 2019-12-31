#pragma once

#include "SSRenderTargetBase.h"
#include "SSTexture2DBase.h"

#include <wrl.h>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

class ENGINE_API SSRenderTargetTexture2D : public SSTexture2DBase
{
public:	
	SSRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, bool bGeneratedMips = false, UINT maxMipCount=5);

	virtual void Resize(const UINT newWidth, const UINT newHeight);
	virtual void Destroy() override;
	virtual void SaveAsDDSFile(std::wstring filename);	
	
	ID3D11RenderTargetView* GetRenderTargetView(UINT mip = 0) { return mRenderTargetView[mip].Get(); }
	virtual void Clear(ID3D11DeviceContext* deviceContext);
	UINT GetMipLevels() const { return mMipLevels; }

protected:
	friend class SSGBuffer;

	ComPtr<ID3D11RenderTargetView> mRenderTargetView[10]{ nullptr };
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format, const UINT mipLevels);
	bool mGenerateMips = false;	
};

class ENGINE_API SSDepthRenderTargetTexture2D : public SSTexture2DBase
{
public:
	SSDepthRenderTargetTexture2D(const UINT width, const UINT height,  DXGI_FORMAT eFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);

	virtual void Resize(const UINT newWidth, const UINT newHeight);
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView.Get(); }
	virtual void Clear(ID3D11DeviceContext* deviceContext);
	virtual void Destroy() override;
protected:
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);

	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
};

class ENGINE_API SSGenericRenderTarget : public IRenderTarget
{
public:
	SSGenericRenderTarget(UINT width, UINT height, UINT count, bool bDepthExist = true, DXGI_FORMAT eFormat = DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_FORMAT eDepthFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);

	SSRenderTargetTexture2D* GetOutput(UINT nIndex);

	void SaveRTTexture(UINT index, std::wstring filename);

	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
	
	virtual void SetCurrentRenderTarget(ID3D11DeviceContext* deviceContext) override;

	virtual void Clear(ID3D11DeviceContext* deviceContext) override;	
	virtual void Resize(UINT width, UINT height) override;

	virtual void Destroy();

protected:
	SSRenderTargetTexture2D* mRenderTargetArray[4]{ nullptr };
	SSDepthRenderTargetTexture2D* mDepthTarget = nullptr;
	ID3D11RenderTargetView** mRenderTargetViews{ nullptr };
	
	D3D11_VIEWPORT mViewport;
	DXGI_FORMAT mFormat;

	UINT mWidth;
	UINT mHeight;
	UINT mCount = 1;
	bool mDepthExist = true;
};