#pragma once

#include "SSRenderThreadObject.h"

class DX11RENDERER_API IRenderTarget
{
public:
	virtual UINT GetRenderTargetWidth() const = 0;
	virtual UINT GetRenderTargetHeight() const = 0;
	virtual void SetCurrentRenderTarget(ID3D11DeviceContext* deviceContext) = 0;	
	//virtual void SetCurrentRenderTarget() = 0;
	virtual void Clear(ID3D11DeviceContext* deviceContext) = 0;	
	virtual void Resize(UINT width, UINT height) = 0;
};
