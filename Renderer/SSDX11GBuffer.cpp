
#include "SSRendererModulePCH.h"
#include "SSDX11GBuffer.h"
#include "SSDX11RenderTarget.h"
#include "SSDX11Renderer.h"


SSDX11GBuffer::SSDX11GBuffer(UINT width, UINT height, DXGI_FORMAT format)
	: SSDX11RenderTarget(width, height, static_cast<UINT>(EGBufferType::Max))
{	
}

SSDX11GBuffer::~SSDX11GBuffer()
{
	Destroy();
}

void SSDX11GBuffer::Destroy()
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