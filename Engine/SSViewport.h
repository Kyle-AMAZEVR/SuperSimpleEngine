

#pragma once

#include "SSRenderTargetBase.h"


class ENGINE_API SSViewport : public SSRenderTargetBase
{
public:
    UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }
    void Clear();

    void Resize(int newWidth, int newHeight);	

	virtual void SetCurrentRenderTarget() override;

protected:

    ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
    D3D11_VIEWPORT mScreenViewport;
};
