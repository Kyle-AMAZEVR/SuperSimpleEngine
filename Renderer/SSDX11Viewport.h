

#pragma once


#include "SSViewport.h"
#include "wrl/client.h"

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11Viewport : public SSViewport
{
public: 		
	SSDX11Viewport(unsigned int inWidth, unsigned int inHeight);
	
    virtual void Resize(SSDX11Device* device, UINT newWidth, UINT newHeight) override;
	virtual void Clear(SSDX11Device* device) override;
	virtual void SetCurrentRenderTarget(SSDX11Device* device) override;
protected:
    ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
	ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
    
	D3D11_VIEWPORT mScreenViewport;
	D3D11_TEXTURE2D_DESC mDepthStencilDesc;

	DXGI_FORMAT mFormat = DXGI_FORMAT_UNKNOWN;
};
