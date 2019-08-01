
#include "Core.h"
#include "SSGBuffer.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSGBuffer::SSGBuffer(UINT width, UINT height, DXGI_FORMAT format)
	: SSGenericRenderTarget(width, height, static_cast<UINT>(EGBufferType::Max), format)
{	
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