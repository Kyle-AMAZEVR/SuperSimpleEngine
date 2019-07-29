#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSRenderTargetBase : public DXRenderResource
{
public:
	SSRenderTargetBase();
	SSRenderTargetBase(UINT width, UINT height, DXGI_FORMAT eFormat);
	virtual void MakeCurrentRenderTarget(){}
	virtual void Resize(UINT width, UINT height){}

protected:
	UINT mWidth;
	UINT mHeight;
	DXGI_FORMAT mFormat;
};
