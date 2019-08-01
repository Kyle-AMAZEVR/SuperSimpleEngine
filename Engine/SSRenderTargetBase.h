#pragma once

#include "DXRenderResource.h"

class ENGINE_API IRenderTarget : public DXRenderResource
{
public:
	virtual UINT GetWidth() const = 0;
	virtual UINT GetHeight() const = 0;
	virtual void SetCurrentRenderTarget() = 0;
	virtual void Clear() = 0;
	virtual void Resize(UINT width, UINT height) = 0;
};
