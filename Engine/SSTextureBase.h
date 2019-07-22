#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSTextureBase : public DXRenderResource
{
public:
	UINT GetWidth() { return mWidth; }
	UINT GetHeight() {		return mHeight; 	}
protected:
	UINT mWidth;
	UINT mHeight;
	
};