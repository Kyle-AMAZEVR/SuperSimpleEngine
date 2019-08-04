#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSTexture2DBase : public DXRenderResource
{
public:

	UINT GetWidth() { return mWidth; }
	UINT GetHeight() {	return mHeight; }
	
	virtual ID3D11ShaderResourceView* GetShaderResourceView() { return mShaderResourceView; }
	virtual ID3D11ShaderResourceView*& GetShaderResourceViewRef() { return mShaderResourceView; }
	
	virtual ID3D11Texture2D* GetTextureResource() { return mTexturePtr; }
	virtual ID3D11Texture2D*& GetTextureResourceRef() { return mTexturePtr; }
	

protected:
	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;
	ID3D11Texture2D* mTexturePtr = nullptr;		
	ID3D11ShaderResourceView* mShaderResourceView = nullptr;
};