#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSRenderTarget2D : public SSTexture2DBase
{
public:
	SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT eFormat);
	virtual void Resize(const UINT newWidth, const UINT newHeight) override;
	virtual void Destroy() override;

	ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }
	ID3D11ShaderResourceView*& GetShaderResourceViewRef() { return mShaderResourceView; }
	
	ID3D11RenderTargetView* GetRenderTargetView() { return mRenderTargetView; }

	void Clear();

protected:

	friend class SSGBuffer;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);	
};

class ENGINE_API SSDepthRenderTarget2D : public SSTexture2DBase
{
public:
	SSDepthRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT eFormat = DXGI_FORMAT_D24_UNORM_S8_UINT);
	virtual void Resize(const UINT newWidth, const UINT newHeight) override;
	ID3D11DepthStencilView* GetDepthStencilView() { return mDepthStencilView; }
	void Clear();
	virtual void Destroy() override;
protected:
	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
};