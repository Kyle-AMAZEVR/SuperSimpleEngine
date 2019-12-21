

#include "Core.h"
#include "SSCubemapRenderTarget.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"
#include "SSMathHelper.h"
#include "DirectXTex.h"

SSCubemapRenderTarget::SSCubemapRenderTarget(UINT width, UINT height, enum DXGI_FORMAT format)	
{
	check(width == height);

	mWidth = width;
	mHeight = height;
	mTextureFormat = format;	
	mMipLevels = SSMathHelper::CalcMipLevel(mWidth);
	
	InternalCreate();
}

void SSCubemapRenderTarget::InternalCreate()
{
	for(int i = 0; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i] = new SSRenderTargetTexture2D(mWidth, mHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	}

	mViewport.TopLeftX = mViewport.TopLeftY = 0;
	mViewport.Width =  static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1.0f;
}

void SSCubemapRenderTarget::Clear()
{
	for(int i = 0 ; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i]->Clear();
	}
}

void SSCubemapRenderTarget::ClearFace(ECubemapFace eFace)
{
	int nFace = static_cast<int>(eFace);
	mRenderTargetArray[nFace]->Clear();
}


void SSCubemapRenderTarget::SetCurrentRTAs(ECubemapFace eFace)
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(eFace)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}


void SSCubemapRenderTarget::CreateCubemapShaderResource()
{
	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth;
	description.Height = mHeight;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = mMipLevels ;
	description.ArraySize = 6;
	description.CPUAccessFlags = 0;
	description.Format = mTextureFormat;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceViewDesc.TextureCube.MostDetailedMip = 0;
	resourceViewDesc.TextureCube.MipLevels = mMipLevels;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));
	//	
	
	for (UINT face = 0; face < 6; ++face)
	{
		auto dstSubresource = D3D11CalcSubresource(0, face, mMipLevels);
		auto srcSubresource = D3D11CalcSubresource(0, 0, mMipLevels);

		SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(mTexturePtr.Get(), dstSubresource, 0, 0, 0, mRenderTargetArray[face]->GetTextureResource(), srcSubresource, nullptr);
	}
	
	SSEngine::Get().GetDeviceContext()->GenerateMips(mShaderResourceView.Get());
}

void SSCubemapRenderTarget::Destroy()
{
	ReleaseCOM(mShaderResourceView);
	ReleaseCOM(mTexturePtr);
	for (UINT i = 0; i < 6; ++i)
	{
		mRenderTargetArray[i]->Destroy();
		delete mRenderTargetArray[i];
	}
}


SSRenderTargetTexture2D* SSCubemapRenderTarget::GetRenderTargetTexture(ECubemapFace eFace)
{
	return mRenderTargetArray[static_cast<int>(eFace)];
}



SSPrefilterCubemapRenderTarget::SSPrefilterCubemapRenderTarget(UINT width, UINT height, UINT maxMipCount, DXGI_FORMAT format)	
	: SSCubemapRenderTarget()
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = format;
	
	check(mWidth == mHeight);
	bool bPowerOfTwo = !(mWidth == 0) && !(mWidth & (mWidth - 1));
	check(bPowerOfTwo);
	check(maxMipCount > 0);

	mMipLevels = maxMipCount;	
	
	InternalCreate();
}

void SSPrefilterCubemapRenderTarget::CreateCubemapShaderResource()
{
	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth;
	description.Height = mHeight;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE | D3D11_RESOURCE_MISC_GENERATE_MIPS;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = mMipLevels;
	description.ArraySize = 6;
	description.CPUAccessFlags = 0;
	description.Format = mTextureFormat;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceViewDesc.TextureCube.MostDetailedMip = 0;
	resourceViewDesc.TextureCube.MipLevels = mMipLevels;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr.Get(), &resourceViewDesc, &mShaderResourceView));

	//
	for (UINT mipLevel = 0; mipLevel < mMipLevels; ++mipLevel)
	{
		for (UINT face = 0; face < 6; ++face)
		{
			auto dstSubresource = D3D11CalcSubresource(mipLevel, face, mMipLevels);
			auto srcSubresource = D3D11CalcSubresource(mipLevel, 0, mMipLevels);

			SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(mTexturePtr.Get(), dstSubresource, 0, 0, 0, mRenderTargetArray[face]->GetTextureResource(), srcSubresource, nullptr);
		}
	}
}

void SSPrefilterCubemapRenderTarget::SetCurrentRTAs(ECubemapFace eFace, UINT mip)
{	
	if (mip != mLastRTMip)
	{
		mViewport.Width = mWidth / static_cast<float>(std::pow(2, mip));
		mViewport.Height = mHeight / static_cast<float>(std::pow(2, mip));
		mLastRTMip = mip;
	}	

	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(eFace)]->GetRenderTargetView(mip) };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSPrefilterCubemapRenderTarget::InternalCreate()
{
	for (int i = 0; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i] = new SSRenderTargetTexture2D(mWidth, mHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, true, mMipLevels);
	}

	mViewport.TopLeftX = mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0;
	mViewport.MaxDepth = 1.0f;
}

// @ save as cubemap 
// @ include all mips
void SSCubemapRenderTarget::SaveAsCubemapDDSFile(std::wstring filename)
{
	const UINT mipTextureWidth = mWidth;
	const UINT mipTextureHeight = mHeight;

	// @create staging texture for copy
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth;
	textureDesc.Height = mHeight;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	textureDesc.MipLevels = mMipLevels;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.ArraySize = 6;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Format = mTextureFormat;
	ID3D11Texture2D* copiedTexturePtr = nullptr;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &copiedTexturePtr));

	SSEngine::Get().GetDeviceContext()->CopyResource(copiedTexturePtr, mTexturePtr.Get());	

	DirectX::Image* imageList = new DirectX::Image[6 * mMipLevels];
	
	for (UINT face = 0; face < 6; face++)
	{
		for (UINT mip = 0; mip < mMipLevels; mip++)
		{
			UINT imageListIndex = face * mMipLevels + mip;

			D3D11_MAPPED_SUBRESOURCE mapped;

			auto srcSubResource = D3D11CalcSubresource(mip, face, mMipLevels);

			HR(SSEngine::Get().GetDeviceContext()->Map(copiedTexturePtr, srcSubResource, D3D11_MAP_READ, 0, &mapped));

			InitD3DDesc(imageList[imageListIndex]);
			imageList[imageListIndex].width = mWidth / static_cast<UINT>(std::pow(2, mip));
			imageList[imageListIndex].height = mHeight / static_cast<UINT>(std::pow(2, mip));
			imageList[imageListIndex].pixels = new uint8_t[mapped.RowPitch * imageList[imageListIndex].height];
			imageList[imageListIndex].rowPitch = mapped.RowPitch;
			imageList[imageListIndex].format = mTextureFormat;
			imageList[imageListIndex].slicePitch = mapped.RowPitch * imageList[imageListIndex].height;

			// copy raw data
			memcpy_s(imageList[imageListIndex].pixels, mapped.RowPitch * imageList[imageListIndex].height, mapped.pData, mapped.RowPitch * imageList[imageListIndex].height);

			SSEngine::Get().GetDeviceContext()->Unmap(copiedTexturePtr, srcSubResource);
		}
	}

	TexMetadata metaData;
	InitD3DDesc(metaData);

	metaData.format = mTextureFormat;
	metaData.mipLevels = mMipLevels;
	metaData.dimension = TEX_DIMENSION_TEXTURE2D;
	metaData.width = mipTextureWidth;
	metaData.height = mipTextureHeight;
	metaData.arraySize = 6;	
	metaData.miscFlags = TEX_MISC_TEXTURECUBE;
		
	HR(DirectX::SaveToDDSFile(imageList, 6 * mMipLevels, metaData, 0, filename.c_str()));

	for (UINT i = 0; i < 6 * mMipLevels; ++i)
	{
		delete[] imageList[i].pixels;
		imageList[i].pixels = nullptr;
	}

	ReleaseCOM(copiedTexturePtr);
}

// @save specific face
// @all mips included
void SSCubemapRenderTarget::SaveFaceAsDDSFile(ECubemapFace eFace)
{
	const UINT mipTextureWidth = mWidth;
	const UINT mipTextureHeight = mHeight;
	const UINT face = static_cast<UINT>(eFace);

	// @create staging texture for copy
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth;
	textureDesc.Height = mHeight;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.MipLevels = mMipLevels;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Format = mTextureFormat;
	ID3D11Texture2D* copiedTexturePtr = nullptr;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &copiedTexturePtr));

	// copy resource 
	// all mips
	for (UINT mip = 0; mip < mMipLevels; ++mip)
	{		
		auto srcSubResource = D3D11CalcSubresource(mip, face, mMipLevels);
		auto dstSubResource = D3D11CalcSubresource(mip, 0, mMipLevels);

		SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(copiedTexturePtr, dstSubResource, 0, 0, 0, mTexturePtr.Get(), srcSubResource, nullptr);
	}	

	DirectX::Image* imageList = new DirectX::Image[mMipLevels];
	// 
	for (UINT mip = 0; mip < mMipLevels; ++mip)
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		
		auto srcSubResource = D3D11CalcSubresource(mip, 0, mMipLevels);
		
		HR(SSEngine::Get().GetDeviceContext()->Map(copiedTexturePtr,srcSubResource, D3D11_MAP_READ, 0, &mapped));
		
		InitD3DDesc(imageList[mip]);
		imageList[mip].width = mWidth / static_cast<UINT>(std::pow(2, mip));
		imageList[mip].height = mHeight / static_cast<UINT>(std::pow(2, mip));
		imageList[mip].pixels = new uint8_t[mapped.RowPitch * imageList[mip].height];
		imageList[mip].rowPitch = mapped.RowPitch;
		imageList[mip].format = mTextureFormat;
		imageList[mip].slicePitch = mapped.RowPitch * imageList[mip].height;

		// copy raw data
		memcpy_s(imageList[mip].pixels, mapped.RowPitch * imageList[mip].height, mapped.pData, mapped.RowPitch * imageList[mip].height);

		SSEngine::Get().GetDeviceContext()->Unmap(copiedTexturePtr, srcSubResource);
	}
	
	TexMetadata metaData;
	InitD3DDesc(metaData);
	
	metaData.format = mTextureFormat;
	metaData.mipLevels = mMipLevels;
	metaData.dimension = TEX_DIMENSION_TEXTURE2D;
	metaData.width = mipTextureWidth;
	metaData.height = mipTextureHeight;
	metaData.arraySize = 1;

	wchar_t buffer[1024];

	wsprintfW(buffer, L"CubemapRT_Face_%d_MipIncluded.dds", static_cast<int>(eFace));

	HR(DirectX::SaveToDDSFile(imageList, mMipLevels, metaData, 0, buffer));

	for (UINT i = 0; i < mMipLevels; ++i)
	{
		delete[] imageList[i].pixels;
		imageList[i].pixels = nullptr;
	}	

	ReleaseCOM(copiedTexturePtr);
}

// @save specific face and specific mip
void SSCubemapRenderTarget::SaveFaceOfMipAsDDSFile(ECubemapFace eFace, UINT mip)
{
	const UINT mipTextureWidth = mWidth / static_cast<UINT>(std::pow(2, mip));
	const UINT mipTextureHeight = mHeight / static_cast<UINT>(std::pow(2, mip));
	const UINT face = static_cast<UINT>(eFace);

	// @create staging texture for copy
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth / static_cast<UINT>(std::pow(2, mip));
	textureDesc.Height = mHeight / static_cast<UINT>(std::pow(2, mip));
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.MipLevels = 1;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Format = mTextureFormat;
	ID3D11Texture2D* copiedTexturePtr = nullptr;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &copiedTexturePtr));

	// copy resource	
	auto srcSubResource = D3D11CalcSubresource(mip, face, mMipLevels);

	SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(copiedTexturePtr, 0, 0, 0, 0, mTexturePtr.Get(), srcSubResource, nullptr);

	D3D11_MAPPED_SUBRESOURCE mapped;
	HR(SSEngine::Get().GetDeviceContext()->Map(copiedTexturePtr, 0, D3D11_MAP_READ, 0, &mapped));

	DirectX::Image image;
	InitD3DDesc(image);
	image.width = mipTextureWidth;
	image.height = mipTextureHeight;
	image.pixels = new uint8_t[mapped.RowPitch * mipTextureHeight];
	image.rowPitch = mapped.RowPitch;
	image.format = mTextureFormat;
	image.slicePitch = mapped.RowPitch * mipTextureHeight;

	// copy raw data
	memcpy_s(image.pixels, mapped.RowPitch * mipTextureHeight, mapped.pData, mapped.RowPitch * mipTextureHeight);

	SSEngine::Get().GetDeviceContext()->Unmap(copiedTexturePtr, 0);

	wchar_t buffer[1024];

	wsprintfW(buffer, L"CubemapRT_Face_%d_Mip_%d.dds", static_cast<int>(eFace), mip);

	HR(DirectX::SaveToDDSFile(image, 0, buffer));

	delete[] image.pixels;

	ReleaseCOM(copiedTexturePtr);
}