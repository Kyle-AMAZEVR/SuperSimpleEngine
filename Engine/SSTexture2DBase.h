#pragma once

#include "SSRenderThreadObject.h"
#include <wrl/internal.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSTexture2DBase : public SSRenderThreadResidentObject
{
public:

	UINT GetWidth() { return mWidth; }
	UINT GetHeight() {	return mHeight; }
	
	virtual ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView.Get(); }
	virtual ID3D11ShaderResourceView* const* GetShaderResourceViewRef() { return mShaderResourceView.GetAddressOf(); }
	
	virtual ID3D11Texture2D* GetTextureResource() { return mTexturePtr.Get(); }
	virtual ID3D11Texture2D* const* GetTextureResourceRef() { return mTexturePtr.GetAddressOf(); }
	

protected:
	UINT mWidth;
	UINT mHeight;
	UINT mMipLevels = 1;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;

	ComPtr<ID3D11Texture2D> mTexturePtr = nullptr;		
	ComPtr<ID3D11ShaderResourceView> mShaderResourceView = nullptr;
	
};