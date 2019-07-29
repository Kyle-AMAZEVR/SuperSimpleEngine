
#pragma once

#include "SSRenderTargetBase.h"

class ENGINE_API SSGBuffer : public SSRenderTargetBase
{
public:
	SSGBuffer(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT::DXGI_FORMAT_R16G16B16A16_FLOAT);

	virtual void Resize(UINT newWidth, UINT newHeight) override;

	virtual void MakeCurrentRenderTarget() override;

	virtual void Destroy() override;

	void Clear();

protected:

	// 
	class SSRenderTarget2D* mRenderTargetArray[3]{ nullptr };
	class SSDepthRenderTarget2D* mDepthTarget = nullptr;
	D3D11_VIEWPORT mViewport;
};
