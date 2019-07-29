
#include "Core.h"
#include "SSGBuffer.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSGBuffer::SSGBuffer(UINT width, UINT height, DXGI_FORMAT format)
{
	mRenderTargetArray[0] = new SSRenderTarget2D(mWidth, mHeight, mFormat);
	mRenderTargetArray[1] = new SSRenderTarget2D(mWidth, mHeight, mFormat);
	mRenderTargetArray[2] = new SSRenderTarget2D(mWidth, mHeight, mFormat);

	
}

void SSGBuffer::Destroy()
{
	delete[] mRenderTargetArray;
}

void SSGBuffer::MakeCurrent()
{
	ID3D11RenderTargetView* renderTargets[3]{
		mRenderTargetArray[0]->GetRenderTargetView(),
		mRenderTargetArray[1]->GetRenderTargetView(),
		mRenderTargetArray[2]->GetRenderTargetView(),
	};

	ID3D11DepthStencilView* depthStencil = mDepthTarget->GetDepthStencilView();
	
	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(3, renderTargets, depthStencil);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}