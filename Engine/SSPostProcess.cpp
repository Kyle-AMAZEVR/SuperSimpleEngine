
#include "Core.h"
#include "SSPostProcess.h"
#include "SSRenderTarget2D.h"
#include "SSScreenBlit.h"

SSPostProcess::SSPostProcess(UINT width, UINT height, UINT rtCount, bool bDepthExist, bool bFixed, DXGI_FORMAT format)
	: mWidth(width), mHeight(height), mRTCount(rtCount), mFixedSize(bFixed), mDepthExist(bDepthExist)
{
	mRenderTarget = std::make_unique< SSGenericRenderTarget>(width, height, rtCount, bDepthExist, format);
	mScreenBlit = std::make_shared<SSScreenBlit>();
}


void SSPostProcess::OnResize(UINT width, UINT height)
{
	if (mFixedSize == false)
	{
		mWidth = width;
		mHeight = height;
		mRenderTarget->Resize(width, height);
	}
}


SSRenderTargetTexture2D* SSPostProcess::GetOutput(UINT nIndex)
{
	if (mRenderTarget != nullptr)
	{
		mRenderTarget->GetOutput(nIndex);
	}

	return nullptr;
}

void SSPostProcess::Destroy()
{
	if(mRenderTarget != nullptr)
	{
		mRenderTarget->Destroy();
	}
}