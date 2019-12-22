

#pragma once

#include "SSRenderTargetBase.h"
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

class ENGINE_API SSViewport : public IRenderTarget
{
public:
    virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
    virtual void Clear() override;
    virtual void Resize(UINT newWidth, UINT newHeight) override;	
	virtual void SetCurrentRenderTarget() override;

protected:

    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    D3D11_VIEWPORT mScreenViewport;

	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
};
