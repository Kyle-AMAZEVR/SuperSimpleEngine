
#include "SSRendererModulePCH.h"
#include "SSDX11PostProcess.h"
#include "SSDX11RenderTarget.h"
#include "SSScreenBlit.h"

SSDX11PostProcess::SSDX11PostProcess(UINT width, UINT height, UINT rtCount, bool bDepthExist, bool bFixed, DXGI_FORMAT format)
	: mWidth(width), mHeight(height), mRTCount(rtCount), mFixedSize(bFixed), mDepthExist(bDepthExist)
{
	mRenderTarget = std::make_unique< SSDX11RenderTarget>(width, height, rtCount, bDepthExist, format);
	mScreenBlit = std::make_shared<SSScreenBlit>();
}


void SSDX11PostProcess::OnResize(UINT width, UINT height)
{
	if (mFixedSize == false)
	{
		mWidth = width;
		mHeight = height;
		mRenderTarget->Resize(width, height);
	}
}


SSDX11RenderTargetTexture2D* SSDX11PostProcess::GetOutput(UINT nIndex)
{
	if (mRenderTarget != nullptr)
	{
		return mRenderTarget->GetOutput(nIndex);
	}

	return nullptr;
}

void SSDX11PostProcess::Destroy()
{
	if(mRenderTarget != nullptr)
	{
		mRenderTarget->Destroy();
	}
}