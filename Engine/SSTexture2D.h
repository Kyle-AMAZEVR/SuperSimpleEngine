#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSTexture2D : public SSTexture2DBase
{
public:
	SSTexture2D();
	

	bool LoadFromDDSFile(std::wstring filename);
	bool LoadFromTGAFile(std::wstring filename);
	bool Release();

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }	
	UINT GetMipLevels() const { return mMipLevels; }
	bool IsSRGB() const { return bSRGB; }

protected:	
	
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;
	UINT mMipLevels = 1;
	bool bSRGB = true;
};