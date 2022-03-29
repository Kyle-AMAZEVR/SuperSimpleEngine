

#pragma once


#include "SSViewport.h"
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11Viewport : public SSViewport
{
public: 		
    virtual void Resize(SSRenderDevice* device, UINT newWidth, UINT newHeight) override;
	virtual void Clear(SSRenderDevice* device) override;
	virtual void SetCurrentRenderTarget(SSRenderDevice* device) override;
protected:
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    D3D11_VIEWPORT mScreenViewport;
	DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
};
