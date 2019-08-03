
#pragma once

#include "SSRenderTargetBase.h"
#include "SSRenderTarget2D.h"

class SSRenderTarget2D;
class SSDepthRenderTargetTexture2D;

enum class EGBufferType : UINT8
{
	Position,
	Color,
	Normal,	
	Max,
};

class ENGINE_API SSGBuffer : public SSGenericRenderTarget
{
public:
	SSGBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

	void Destroy();
	
	SSRenderTargetTexture2D* GetPositionOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Position)]; }
	SSRenderTargetTexture2D* GetColorOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Color)]; }
	SSRenderTargetTexture2D* GetNormalOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Normal)]; }	
	
};
