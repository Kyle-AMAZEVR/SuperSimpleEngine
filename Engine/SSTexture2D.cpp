


#include "Core.h"
#include "SSTexture2D.h"
#include "stb_image.h"
#include "DXEngine.h"
#include "DirectXTex.h"

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

bool SSTexture2D::LoadFromFile(std::wstring filename)
{		
	//HR(DirectX::CreateDDSTextureFromFile(DXEngine::Get().GetDevice(), filename.c_str(), &mTexturePtr, &mResourceView));
	//HR(DirectX::CreateDDSTextureFromFileEx(
	//	DXEngine::Get().GetDevice(), filename.c_str(), 2048, D3D11_USAGE_DEFAULT, D3D11_BIND_SHADER_RESOURCE, 0, 0,
	//	true, &mTexturePtr, &mResourceView));
	
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;
	HR(DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));

	auto dim = metaData.dimension;
	auto size = metaData.format;

	auto* pImage = image.GetImage(0, 0, 0);
		
	D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(D3D11_SUBRESOURCE_DATA));
	textureData.pSysMem = pImage->pixels;
	textureData.SysMemPitch = pImage->rowPitch;
	textureData.SysMemSlicePitch = pImage->slicePitch;

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = pImage->width;
	description.Height = mHeight = pImage->height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = 1;
	description.ArraySize = 1;
	description.CPUAccessFlags = 0;

	description.Format = DirectX::MakeSRGB(metaData.format);
	
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