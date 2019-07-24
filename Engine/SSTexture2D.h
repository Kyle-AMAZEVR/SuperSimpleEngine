#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSTexture2D : public SSTexture2DBase
{
public:
	SSTexture2D();
	ID3D11ShaderResourceView* GetShaderResourceView() { return mResourceView; }
	ID3D11ShaderResourceView*& GetShaderResourceViewRef() { return mResourceView; }

	bool LoadFromDDSFile(std::wstring filename);
	bool LoadFromTGAFile(std::wstring filename);
	bool Release();

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }	
	UINT GetMipLevels() const { return mMipLevels; }
	bool IsSRGB() const { return bSRGB; }

protected:	
	ID3D11ShaderResourceView* mResourceView = nullptr;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;
	UINT mMipLevels = 1;
	bool bSRGB = true;
};