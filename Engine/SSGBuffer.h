
#pragma once

#include "SSRenderTargetBase.h"

class SSRenderTarget2D;
class SSDepthRenderTarget2D;

enum class EGBufferType : UINT8
{
	Position,
	Color,
	Normal,	
	Max,
};

class ENGINE_API SSGBuffer : public SSRenderTargetBase
{
public:
	SSGBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

	virtual void Resize(UINT newWidth, UINT newHeight) override;

	virtual void SetCurrentRenderTarget() override;

	virtual void UnsetCurrentRenderTarget() override;

	virtual void Destroy() override;

	void Clear();
	
	SSRenderTarget2D* GetPositionOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Position)]; }
	SSRenderTarget2D* GetColorOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Color)]; }
	SSRenderTarget2D* GetNormalOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Normal)]; }
	//SSRenderTarget2D* GetTexcoordOutput() { return mRenderTargetArray[static_cast<UINT8>(EGBufferType::Texcoord)]; }

protected:

	// 
	SSRenderTarget2D* mRenderTargetArray[EGBufferType::Max]{ nullptr };
	SSDepthRenderTarget2D* mDepthTarget = nullptr;
	D3D11_VIEWPORT mViewport;
};
