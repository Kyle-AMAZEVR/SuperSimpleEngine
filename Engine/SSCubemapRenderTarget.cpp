

#include "Core.h"
#include "SSCubemapRenderTarget.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"
#include "DXMathHelper.h"

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

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mShaderResourceView));
	//	
	
	for (UINT face = 0; face < 6; ++face)
	{
		auto dstSubresource = D3D11CalcSubresource(0, face, mMipLevels);
		auto srcSubresource = D3D11CalcSubresource(0, 0, mMipLevels);

		SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(mTexturePtr, dstSubresource, 0, 0, 0, mRenderTargetArray[face]->GetTextureResource(), srcSubresource, nullptr);
	}
	
	SSEngine::Get().GetDeviceContext()->GenerateMips(mShaderResourceView);
}

void SSCubemapRenderTarget::Destroy()
{
	Destroy();
}


SSRenderTargetTexture2D* SSCubemapRenderTarget::GetRenderTargetTexture(ECubemapFace eFace)
{
	return mRenderTargetArray[static_cast<int>(eFace)];
}


SSPrefilterCubemapRenderTarget::SSPrefilterCubemapRenderTarget(UINT width, UINT height, UINT maxMipCount, DXGI_FORMAT format)	
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

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mShaderResourceView));

	//
	for (UINT mipLevel = 0; mipLevel < mMipLevels; ++mipLevel)
	{
		for (UINT face = 0; face < 6; ++face)
		{
			auto dstSubresource = D3D11CalcSubresource(mipLevel, face, mMipLevels);
			auto srcSubresource = D3D11CalcSubresource(mipLevel, 0, mMipLevels);

			SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(mTexturePtr, dstSubresource, 0, 0, 0, mRenderTargetArray[face]->GetTextureResource(), srcSubresource, nullptr);
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