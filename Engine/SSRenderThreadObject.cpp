
#include "Core.h"
#include "SSRenderThreadObject.h"
#include "SSEngine.h"
#include "SSRenderingThread.h"

SSRenderThreadObject::SSRenderThreadObject()
{
    check(SSDX11Engine::IsInitialized() == true);
    check(SSRenderingThread::IsInRenderingThread() == true);
}

SSRenderThreadObject::~SSRenderThreadObject()
{
	check(SSDX11Engine::IsInitialized() == true);
	check(SSRenderingThread::IsInRenderingThread() == true);
}