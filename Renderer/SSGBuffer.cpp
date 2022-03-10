
#include "SSRendererModulePCH.h"
#include "SSGBuffer.h"
#include "SSRenderTarget2D.h"
#include "SSDX11Renderer.h"


SSGBuffer::SSGBuffer(UINT width, UINT height, DXGI_FORMAT format)
	: SSDX11RenderTarget(width, height, static_cast<UINT>(EGBufferType::Max))
{	
}

void SSGBuffer::Destroy()
{
	for (UINT8 i = 0; i < static_cast<UINT8>(EGBufferType::Max); ++i)
	{
		mRenderTargetArray[i]->Destroy();
		delete mRenderTargetArray[i];
		mRenderTargetArray[i] = nullptr;
	}
	
	mDepthTarget->Destroy();
	
	delete mDepthTarget;
	mDepthTarget = nullptr;

}