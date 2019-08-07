

#include "Core.h"
#include "SSCubemapRenderTarget.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSCubemapRenderTarget::SSCubemapRenderTarget(UINT width, UINT height, enum DXGI_FORMAT format)	
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = format;

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


void SSCubemapRenderTarget::Resize(UINT width, UINT height)
{
	for (int i = 0; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i]->Resize(width, height);
	}
}

void SSCubemapRenderTarget::SetCurrentRTAs(ECubemapFace eFace)
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(eFace)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::SetCurrentRTAs(ECubemapFace eFace, UINT mip)
{
	UINT mipWidth, mipHeight;
	if (mRenderTargetArray[static_cast<int>(eFace)]->GetWidth() != mipWidth || mRenderTargetArray[static_cast<int>(eFace)]->GetHeight() != mipHeight)
	{
		mRenderTargetArray[static_cast<int>(eFace)]->Resize(mipWidth, mipHeight);
	}
	
	SetCurrentRTAs(eFace);
}

void SSCubemapRenderTarget::SetCurrentRTAsNegativeX()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::NEGATIVE_X)]->GetRenderTargetView() };
		
	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::SetCurrentRTAsPositiveX()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::POSITIVE_X)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::SetCurrentRTAsNegativeY()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::NEGATIVE_Y)]->GetRenderTargetView()};

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::SetCurrentRTAsPositiveY()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::POSITIVE_Y)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}


void SSCubemapRenderTarget::SetCurrentRTAsNegativeZ()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::NEGATIVE_Z)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::SetCurrentRTAsPositiveZ()
{
	ID3D11RenderTargetView* renderTarget[1]{ mRenderTargetArray[static_cast<int>(ECubemapFace::POSITIVE_Z)]->GetRenderTargetView() };

	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(1, renderTarget, nullptr);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSCubemapRenderTarget::CreateCubemapResource()
{
	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth ;
	description.Height = mHeight;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	description.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.SampleDesc.Quality = 0;
	description.MipLevels = 1;
	description.ArraySize = 6;
	description.CPUAccessFlags = 0 ;
	description.Format = mTextureFormat;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));

	D3D11_SHADER_RESOURCE_VIEW_DESC resourceViewDesc;
	ZeroMemory(&resourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	resourceViewDesc.Format = description.Format;
	resourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	resourceViewDesc.Texture2D.MostDetailedMip = 0;
	resourceViewDesc.Texture2D.MipLevels = 1;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &resourceViewDesc, &mShaderResourceView));

	for (UINT face = 0; face < 6; ++face)
	{
		auto dstSubresource = D3D11CalcSubresource(0, face, 1);

		SSEngine::Get().GetDeviceContext()->CopySubresourceRegion(mTexturePtr, dstSubresource, 0, 0, 0, mRenderTargetArray[face]->GetTextureResource(), 0, nullptr);
	}	
}

void SSCubemapRenderTarget::Destroy()
{
	Destroy();
}


SSRenderTargetTexture2D* SSCubemapRenderTarget::GetRenderTargetTexture(ECubemapFace eFace)
{
	return mRenderTargetArray[static_cast<int>(eFace)];
}