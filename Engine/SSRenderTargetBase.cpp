

#include "Core.h"
#include "SSRenderTargetBase.h"

SSRenderTargetBase::SSRenderTargetBase()
	: mWidth(1024), mHeight(768), mFormat(DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT)
{	
}


SSRenderTargetBase::SSRenderTargetBase(UINT width, UINT height, DXGI_FORMAT eFormat)
	: mWidth(width), mHeight(height), mFormat(eFormat)
{	
}

