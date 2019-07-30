
#include "Core.h"
#include "SSGBuffer.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSGBuffer::SSGBuffer(UINT width, UINT height, DXGI_FORMAT format)
	: SSRenderTargetBase(width, height, format)
{
	mRenderTargetArray[0] = new SSRenderTarget2D(mWidth, mHeight, mFormat);
	mRenderTargetArray[1] = new SSRenderTarget2D(mWidth, mHeight, mFormat);
	mRenderTargetArray[2] = new SSRenderTarget2D(mWidth, mHeight, mFormat);
	//mRenderTargetArray[3] = new SSRenderTarget2D(mWidth, mHeight, mFormat);

	mDepthTarget = new SSDepthRenderTarget2D(mWidth, mHeight);

	// Set the viewport transform.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void SSGBuffer::Destroy()
{
	for (UINT8 i = 0; i < static_cast<UINT8>(EGBufferType::Max); ++i)
	{
		mRenderTargetArray[i]->Destroy();
	}

	delete[] mRenderTargetArray;
	
	mDepthTarget->Destroy();
	
	delete mDepthTarget;
	mDepthTarget = nullptr;

}

void SSGBuffer::Resize(UINT newWidth, UINT newHeight)
{
	mWidth = newWidth;
	mHeight = newHeight;

	mRenderTargetArray[0]->Resize(newWidth, newHeight);
	mRenderTargetArray[1]->Resize(newWidth, newHeight);
	mRenderTargetArray[2]->Resize(newWidth, newHeight);
	//mRenderTargetArray[3]->Resize(newWidth, newHeight);
	mDepthTarget->Resize(newWidth, newHeight);

	// Set the viewport transform.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;

	MakeCurrentRenderTarget();
}

void SSGBuffer::MakeCurrentRenderTarget()
{
	ID3D11RenderTargetView* renderTargets[4]{
		mRenderTargetArray[0]->GetRenderTargetView(),
		mRenderTargetArray[1]->GetRenderTargetView(),
		mRenderTargetArray[2]->GetRenderTargetView(),
		//mRenderTargetArray[3]->GetRenderTargetView(),
	};

	ID3D11DepthStencilView* depthStencil = mDepthTarget->GetDepthStencilView();
	
	SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(3, renderTargets, depthStencil);

	SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
}

void SSGBuffer::Clear()
{	
	mRenderTargetArray[0]->Clear();
	mRenderTargetArray[1]->Clear();
	mRenderTargetArray[2]->Clear();
	//mRenderTargetArray[3]->Clear();
	mDepthTarget->Clear();
}