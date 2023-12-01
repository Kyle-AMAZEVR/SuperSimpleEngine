
#pragma once

#include "SSRenderTargetBase.h"
#include "SSDX11RenderTarget.h"

class SSRenderTarget2D;
class SSDepthRenderTargetTexture2D;

enum class EGBufferType : UINT8
{
	Color,
	Position,
	Normal,	
	Max,
};

class DX11RENDERER_API SSDX11GBuffer : public SSDX11RenderTarget
{
public:
	SSDX11GBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);
	virtual ~SSDX11GBuffer();

	void Destroy();
	
	SSDX11RenderTargetTexture2D* GetPositionOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Position)]; }
	SSDX11RenderTargetTexture2D* GetColorOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Color)]; }
	SSDX11RenderTargetTexture2D* GetNormalOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Normal)]; }	
	
};
