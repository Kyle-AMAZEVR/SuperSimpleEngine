#pragma once

#include "SSRenderCore.h"
#include "SSBufferBase.h"

class RENDERCORE_API SSConstantBuffer : public SSBufferBase
{
public:


protected:
	BYTE* mBufferData = nullptr;
};