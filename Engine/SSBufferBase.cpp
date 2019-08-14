
#include "Core.h"
#include "SSBufferBase.h"
#include "SSEngine.h"


SSBufferBase::~SSBufferBase()
{
	Destroy();
}

void SSBufferBase::Destroy()
{
	ReleaseCOM(mpBuffer);
}
