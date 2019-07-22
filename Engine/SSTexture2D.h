#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSTexture2D : public DXRenderResource
{
public:
	SSTexture2D();
	ID3D11ShaderResourceView* GetShaderResourceView() { return mResourceView; }

	bool LoadFromFile(std::string filename);
	bool Release();

protected:
	UINT mWidth = 0;
	UINT mHeight = 0;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	
	ID3D11Texture2D* mTexturePtr = nullptr;
	ID3D11ShaderResourceView* mResourceView = nullptr;
};