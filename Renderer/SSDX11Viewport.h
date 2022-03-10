

#pragma once

#include "SSDX11Device.h"
#include "SSRenderTargetBase.h"
#include "SSViewport.h"
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11Viewport : public SSViewport
{
public: 		
    virtual void Resize(UINT newWidth, UINT newHeight) override;
	virtual void Clear() override;

	void SetCurrentRenderTarget(SSDX11Device* device);

protected:
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    D3D11_VIEWPORT mScreenViewport;
	DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
};
