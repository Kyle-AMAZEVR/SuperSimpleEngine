
#include "Core.h"
#include "DXRenderResource.h"
#include "SSEngine.h"
#include "SSRenderingThread.h"

DXRenderResource::DXRenderResource()
{
    check(SSDX11Engine::IsInitialized() == true);
    check(SSRenderingThread::IsInRenderingThread() == true);
}

DXRenderResource::~DXRenderResource()
{
	check(SSDX11Engine::IsInitialized() == true);
	check(SSRenderingThread::IsInRenderingThread() == true);
}