
#include "Core.h"
#include "SSBufferBase.h"
#include "SSEngine.h"


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
