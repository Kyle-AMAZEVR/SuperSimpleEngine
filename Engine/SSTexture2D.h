#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSTexture2D : public DXRenderResource
{
public:
	SSTexture2D();	
	bool LoadFromFile(std::string filename);

protected:
	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
	ID3D11Texture2D* mTexturePtr = nullptr;
};