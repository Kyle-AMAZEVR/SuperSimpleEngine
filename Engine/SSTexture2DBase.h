#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSTexture2DBase : public DXRenderResource
{
public:

	UINT GetWidth() { return mWidth; }
	UINT GetHeight() {	return mHeight; }
	ID3D11Texture2D* GetTexturePtr() { return mTexturePtr; }

	virtual void Resize(const UINT newWidth, const UINT newHeight){}

protected:
	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mTextureFormat;
	ID3D11Texture2D* mTexturePtr = nullptr;	
	
};