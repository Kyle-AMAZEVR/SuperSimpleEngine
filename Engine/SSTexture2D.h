#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSTexture2D : public SSTexture2DBase
{
public:
	SSTexture2D();
	ID3D11ShaderResourceView* GetShaderResourceView() { return mResourceView; }
	ID3D11ShaderResourceView*& GetShaderResourceViewRef() { return mResourceView; }

	bool LoadFromFile(std::wstring filename);
	bool Release();
	UINT GetWidth() { return mWidth; }
	UINT GetHeight() { return mHeight; }	

protected:	
	
	ID3D11ShaderResourceView* mResourceView = nullptr;
};