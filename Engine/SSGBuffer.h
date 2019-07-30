
#pragma once

#include "SSRenderTargetBase.h"

class SSRenderTarget2D;
class SSDepthRenderTarget2D;

class ENGINE_API SSGBuffer : public SSRenderTargetBase
{
public:
	SSGBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

	virtual void Resize(UINT newWidth, UINT newHeight) override;

	virtual void MakeCurrentRenderTarget() override;

	virtual void Destroy() override;

	void Clear();
	
	SSRenderTarget2D* GetPositionOutput() { return mRenderTargetArray[0]; }
	SSRenderTarget2D* GetColorOutput() { return mRenderTargetArray[1]; }
	SSRenderTarget2D* GetNormalOutput() { return mRenderTargetArray[2]; }

protected:

	// 
	SSRenderTarget2D* mRenderTargetArray[3]{ nullptr };
	SSDepthRenderTarget2D* mDepthTarget = nullptr;
	D3D11_VIEWPORT mViewport;
};
