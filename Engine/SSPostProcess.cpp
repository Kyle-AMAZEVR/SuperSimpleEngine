
#include "Core.h"
#include "SSPostProcess.h"
#include "SSRenderTarget2D.h"

SSPostProcess::SSPostProcess(UINT width, UINT height, UINT rtCount, bool bDepthExist, bool bFixed, DXGI_FORMAT format)
	: mWidth(width), mHeight(height), mRTCount(rtCount), mFixedSize(bFixed), mDepthExist(bDepthExist)
{
	mRenderTarget = std::make_unique< SSGenericRenderTarget>(width, height, rtCount, bDepthExist, format);
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


