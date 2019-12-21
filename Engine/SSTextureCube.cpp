
#include "Core.h"
#include "SSTextureCube.h"
#include "DirectXTex.h"
#include "SSEngine.h"

std::shared_ptr<SSTextureCube> SSTextureCube::CreateFromDDSFile(std::wstring filename)
{
	std::shared_ptr<SSTextureCube> texture = std::make_shared<SSTextureCube>();
	
	if (texture->LoadFromDDSFile(filename))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}	
}

bool SSTextureCube::LoadFromDDSFile(std::wstring filename)
{
	check(mTexturePtr == nullptr);

	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;	

	HRESULT result = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image);

	if (result != S_OK)
	{
		return false;
	}
	
	mWidth = static_cast<UINT>(metaData.width);
	mHeight = static_cast<UINT>(metaData.height);
	mMipLevels = static_cast<UINT>(metaData.mipLevels);

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = static_cast<UINT>(metaData.width);
	description.Height = mHeight = static_cast<UINT>(metaData.height);
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = static_cast<UINT>(metaData.mipLevels);
	description.ArraySize = static_cast<UINT>(metaData.arraySize);
	description.CPUAccessFlags = 0;
	
	if (bSRGB)
	{
		mTextureFormat = description.Format = DirectX::MakeSRGB(metaData.format);
	}
	else
	{
		mTextureFormat = description.Format = metaData.format;
	}

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));
	
	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));

	for (UINT face = 0; face < 6; ++face)
	{
		for (UINT mipLevel = 0; mipLevel < description.MipLevels; ++mipLevel)
		{
			auto* pLodImage = image.GetImage(mipLevel, face, 0);
			check(pLodImage != nullptr);

			auto dstSubresource = D3D11CalcSubresource(mipLevel, face, metaData.mipLevels);			
			SSEngine::Get().GetDeviceContext()->UpdateSubresource(mTexturePtr.Get(), dstSubresource, nullptr, pLodImage->pixels, pLodImage->rowPitch, 0);
		}
	}	

	return true;
}