
#include "Core.h"
#include "SSTextureCube.h"
#include "DirectXTex.h"
#include "SSEngine.h"

bool SSTextureCube::LoadFromHDRFile(std::wstring filename)
{
	
	   
	return true;
}

bool SSTextureCube::LoadFromDDSFiles(std::vector<std::wstring> sixFaceFiles)
{
	DirectX::TexMetadata metaDataList[6];
	DirectX::ScratchImage imageList[6];

	for (size_t i = 0; i < sixFaceFiles.size(); ++i)
	{
		HR(DirectX::LoadFromDDSFile(sixFaceFiles[i].c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaDataList[i], imageList[i]));
	}
	
	return true;
}


bool SSTextureCube::LoadFromDDSFile(std::wstring filename)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;	

	HR(DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));
	
	mWidth = metaData.width;
	mHeight = metaData.height;
	mMipLevels = metaData.mipLevels;

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = metaData.width;
	description.Height = mHeight = metaData.height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = metaData.mipLevels;
	description.ArraySize = metaData.arraySize;
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
	resourceViewDesc.Texture2D.MipLevels = metaData.mipLevels;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mShaderResourceView));

	for (UINT face = 0; face < 6; ++face)
	{
		for (UINT mipLevel = 0; mipLevel < description.MipLevels; ++mipLevel)
		{
			auto* pLodImage = image.GetImage(mipLevel, face, 0);
			assert(pLodImage != nullptr);

			auto dstSubresource = D3D11CalcSubresource(mipLevel, face, metaData.mipLevels);			
			SSEngine::Get().GetDeviceContext()->UpdateSubresource(mTexturePtr, dstSubresource, nullptr, pLodImage->pixels, pLodImage->rowPitch, 0);
		}
	}

	

	return true;
}