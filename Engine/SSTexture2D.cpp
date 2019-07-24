


#include "Core.h"
#include "SSTexture2D.h"
#include "stb_image.h"
#include "SSEngine.h"
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

bool SSTexture2D::LoadFromTGAFile(std::wstring filename)
{
	return true;
}

bool SSTexture2D::LoadFromDDSFile(std::wstring filename)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HR(DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));

	assert(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	mWidth = metaData.width;
	mHeight = metaData.height;
	mMipLevels = metaData.mipLevels;


	/*D3D11_SUBRESOURCE_DATA textureData;
	ZeroMemory(&textureData, sizeof(D3D11_SUBRESOURCE_DATA));
	textureData.pSysMem = pImage->pixels;
	textureData.SysMemPitch = pImage->rowPitch;
	textureData.SysMemSlicePitch = pImage->slicePitch;*/

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = metaData.width;
	description.Height = mHeight = metaData.height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE ;	
	description.MiscFlags = 0;
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
	
	//
	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = metaData.mipLevels;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mResourceView));

	// update lod data
	for (int i = 0; i < metaData.mipLevels; ++i)
	{
		auto* pLodImage = image.GetImage(i, 0, 0);
		assert(pLodImage != nullptr);
		auto dstSubresource = D3D11CalcSubresource(i, 0, i);
		SSEngine::Get().GetDeviceContext()->UpdateSubresource(mTexturePtr, dstSubresource, nullptr, pLodImage->pixels, pLodImage->rowPitch, 0);
	}	
	

	return true;
}