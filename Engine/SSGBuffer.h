
#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSGBuffer : public DXRenderResource
{
public:
	SSGBuffer(UINT width, UINT height, DXGI_FORMAT format);

	void MakeCurrent();

	virtual void Destroy() override;
protected:

	// 
	class SSRenderTarget2D* mRenderTargetArray[3]{ nullptr };
	class SSDepthRenderTarget2D* mDepthTarget = nullptr;
	D3D11_VIEWPORT mViewport;

	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat;

};