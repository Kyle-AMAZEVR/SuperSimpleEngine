
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
	if (mTexturePtr != nullptr)
	{
		mTexturePtr->Release();
		mTexturePtr = nullptr;
	}
	if (mResourceView != nullptr)
	{
		mResourceView->Release();
		mResourceView = nullptr;
	}

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
	textureData.SysMemPitch = width * channels;
	textureData.SysMemSlicePitch = width * height * channels;

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = width;
	description.Height = mHeight = height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DYNAMIC;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;	
	
	description.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	
	HR(DXEngine::Get().GetDevice()->CreateTexture2D(&description, &textureData, &mTexturePtr));
	
	HR(DXEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, nullptr, &mResourceView));

	return true;
}