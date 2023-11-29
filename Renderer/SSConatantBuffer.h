#pragma once

#include "SSRenderCore.h"
#include "SSBufferBase.h"

class DX11RENDERER_API SSConstantBuffer : public SSBufferBase
{
public:


protected:
	BYTE* mBufferData = nullptr;
};