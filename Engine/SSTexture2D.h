#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSTexture2D : public SSTexture2DBase
{
public:
	SSTexture2D();
	
	static std::shared_ptr<SSTexture2D> CreateFromDDSFile(std::wstring filename);
	static std::shared_ptr<SSTexture2D> CreateFromHDRFile(std::wstring filename);

	bool LoadFromDDSFile(std::wstring filename);
	bool LoadFromTGAFile(std::wstring filename);
	bool LoadFromHDRFile(std::wstring filename);

	bool Release();

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }	
	UINT GetMipLevels() const { return mMipLevels; }
	bool IsSRGB() const { return bSRGB; }

protected:		
	
	bool bSRGB = true;
};