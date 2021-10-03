


#include "Core.h"
#include "SSTexture2D.h"
#include "SSDX11Renderer.h"

SSTexture2D::SSTexture2D()
{	
}

bool SSTexture2D::Release()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mShaderResourceView);

	return true;
}


bool SSTexture2D::LoadFromHDRFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = DirectX::LoadFromHDRFile(filename.c_str(), &metaData, image);

	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(deviceContext, metaData, image, bsrgb);
}

bool SSTexture2D::LoadFromTGAFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = DirectX::LoadFromTGAFile(filename.c_str(), &metaData, image);
	

	if(result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(deviceContext, metaData, image, bsrgb);
}

bool SSTexture2D::LoadFromDDSFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;	

	HRESULT result = (DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));
	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	return LoadInternal(deviceContext, metaData, image, bsrgb);
}


bool SSTexture2D::LoadInternal(ID3D11DeviceContext* deviceContext, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bsrgb)
{
	mWidth = static_cast<UINT>(metaData.width);
	mHeight = static_cast<UINT>(metaData.height);
	mMipLevels = static_cast<UINT>(metaData.mipLevels);

	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = static_cast<UINT>(metaData.width);
	description.Height = mHeight = static_cast<UINT>(metaData.height);
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = static_cast<UINT>(metaData.mipLevels);
	description.ArraySize = static_cast<UINT>(metaData.arraySize);
	description.CPUAccessFlags = 0;

	this->bSRGB = bsrgb;

	if (bSRGB)
	{
		mTextureFormat = description.Format = DirectX::MakeSRGB(metaData.format);
	}
	else
	{
		mTextureFormat = description.Format = metaData.format;
	}

	//
	HR(SSDX11Renderer::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = static_cast<UINT>(metaData.mipLevels);

	HR(SSDX11Renderer::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));

	// update lod data
	for (int i = 0; i < metaData.mipLevels; ++i)
	{
		auto* pLodImage = image.GetImage(i, 0, 0);
		check(pLodImage != nullptr);

		auto dstSubresource = D3D11CalcSubresource(i, 0, static_cast<UINT>(metaData.mipLevels));		
		check(deviceContext != nullptr);

		deviceContext->UpdateSubresource(mTexturePtr.Get(), dstSubresource, nullptr, pLodImage->pixels, static_cast<UINT>(pLodImage->rowPitch), 0);
	}

	return true;
}

std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromDDSFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSTexture2D> texture = std::make_shared<SSTexture2D>();

	if (texture->LoadFromDDSFile(deviceContext, filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromDDSFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSTexture2D::CreateFromDDSFile(deviceContext, wfilename, bsrgb);
}


std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromTGAFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSTexture2D> texture = std::make_shared<SSTexture2D>();

	if (texture->LoadFromTGAFile(deviceContext, filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}


std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromHDRFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bsrgb)
{
	std::shared_ptr<SSTexture2D> texture = std::make_shared<SSTexture2D>();

	if (texture->LoadFromHDRFile(deviceContext, filename, bsrgb))
	{
		return texture;
	}
	else
	{
		return nullptr;
	}
}

std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromHDRFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSTexture2D::CreateFromHDRFile(deviceContext, wfilename, bsrgb);
}


std::shared_ptr<SSTexture2D> SSTexture2D::CreateFromTGAFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bsrgb)
{
	std::wstring wfilename;
	wfilename.assign(filename.begin(), filename.end());

	return SSTexture2D::CreateFromTGAFile(deviceContext, wfilename, bsrgb);
}