
#include "Core.h"
#include "DXRenderResource.h"
#include "SSEngine.h"
#include "DXRenderingThread.h"

DXRenderResource::DXRenderResource()
{
    check(SSDX11Engine::IsInitialized() == true);
    check(DXRenderingThread::IsInRenderingThread() == true);
}

DXRenderResource::~DXRenderResource()
{
	check(SSDX11Engine::IsInitialized() == true);
	check(DXRenderingThread::IsInRenderingThread() == true);
}