
#define STB_IMAGE_IMPLEMENTATION

#include "Core.h"
#include "SSTexture2D.h"
#include "stb_image.h"
#include "DXEngine.h"

SSTexture2D::SSTexture2D()
{
}

bool SSTexture2D::Release()
{
	return true;
}

bool SSTexture2D::LoadFromFile(std::string filename)
{
	int width, height, channels;
	
	stbi_uc* data = stbi_load(filename.c_str(), &width, &height, &channels, STBI_rgb_alpha);
	
	if (data == nullptr)
	{
		return false;
	}

	D3D11_SUBRESOURCE_DATA textureData;
	textureData.pSysMem = data;	

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = width;
	description.Height = mHeight = height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DYNAMIC;
	description.SampleDesc.Count = 1;
	description.MipLevels = description.ArraySize = 1;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	
	HR(DXEngine::Get().GetDevice()->CreateTexture2D(&description, &textureData, &mTexturePtr));
	
	HR(DXEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, nullptr, &mResourceView));

	return true;
}