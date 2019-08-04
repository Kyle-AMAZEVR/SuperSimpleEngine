

#include "Core.h"
#include "SSCubemapRenderTarget.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSCubemapRenderTarget::SSCubemapRenderTarget(UINT width, UINT height, enum DXGI_FORMAT format)
	: mWidth(width), mHeight(height)
{
	InternalCreate();
}

void SSCubemapRenderTarget::InternalCreate()
{
	for(int i = 0; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i] = new SSRenderTargetTexture2D(mWidth, mHeight, DXGI_FORMAT_R16G16B16A16_FLOAT);
	}

	mViewport.TopLeftX = mViewport.TopLeftY = 0;
	mViewport.Width = mWidth;
	mViewport.Height = mHeight;
}

void SSCubemapRenderTarget::Clear()
{
	for(int i = 0 ; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i]->Clear();
	}
}


void SSCubemapRenderTarget::Resize(UINT width, UINT height)
{
	for (int i = 0; i < static_cast<int>(ECubemapFace::MAX); ++i)
	{
		mRenderTargetArray[i]->Resize(width, height);
	}
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