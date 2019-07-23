


#include "Core.h"
#include "SSTexture2D.h"
#include "stb_image.h"
#include "DXEngine.h"
#include "DDSTextureLoader.h"

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
	
	FILE *f = stbi__fopen(filename.c_str(), "rb");
	stbi__uint16* data = stbi_load_from_file_16(f, &width, &height, &channels, STBI_rgb);

	if (data == nullptr)
	{
		return false;
	}

	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(D3D11_SUBRESOURCE_DATA));
	textureData.pSysMem = data;
	textureData.SysMemPitch = width * channels;
	textureData.SysMemSlicePitch = 0;

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = width;
	description.Height = mHeight = height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.CPUAccessFlags = 0;

	description.Format = DXGI_FORMAT_R16G16B16A16_UNORM;
	
	HR(DXEngine::Get().GetDevice()->CreateTexture2D(&description, &textureData, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 1;
	
	HR(DXEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mResourceView));

	return true;
}