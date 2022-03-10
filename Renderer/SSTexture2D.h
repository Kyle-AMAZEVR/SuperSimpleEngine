#pragma once

#include "SSDX11Texture2D.h"
#include "DirectXTex.h"

class DX11RENDERER_API SSTexture2D : public SSDX11Texture2D
{
public:
	SSTexture2D();	
	
	

	bool Release();

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }	
	UINT GetMipLevels() const { return mMipLevels; }
	bool IsSRGB() const { return bSRGB; }

protected:		
	bool LoadInternal(ID3D11DeviceContext* deviceContext, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bsrgb);
	//bool bSRGB = false;
	bool bSRGB = false;

	
};