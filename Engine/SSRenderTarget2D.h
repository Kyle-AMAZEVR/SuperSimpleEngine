#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSRenderTarget2D : public SSTexture2DBase
{
public:
	SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT eFormat);
	virtual void Resize(const UINT newWidth, const UINT newHeight) override;
	void Release();
protected:
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;

	void InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format);
	
};


class ENGINE_API SSDepthRenderTarget2D : public SSTexture2DBase
{

};