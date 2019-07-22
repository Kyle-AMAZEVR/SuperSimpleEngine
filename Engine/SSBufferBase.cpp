
#include "Core.h"
#include "SSBufferBase.h"
#include "DXEngine.h"


SSBufferBase::~SSBufferBase()
{
	Destroy();
}

void SSBufferBase::Destroy()
{
	if (mpBuffer != nullptr)
	{
		mpBuffer->Release();
		mpBuffer = nullptr;
	}
}
