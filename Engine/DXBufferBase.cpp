
#include "Core.h"
#include "DXBufferBase.h"
#include "DXEngine.h"


DXBufferBase::~DXBufferBase()
{
	Destroy();
}

void DXBufferBase::Destroy()
{
	if (mpBuffer != nullptr)
	{
		mpBuffer->Release();
		mpBuffer = nullptr;
	}
}
