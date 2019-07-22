
#include "Core.h"
#include "DXBufferBase.h"
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
