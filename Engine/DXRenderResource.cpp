
#include "Core.h"
#include "DXRenderResource.h"
#include "SSEngine.h"
#include "DXRenderingThread.h"

DXRenderResource::DXRenderResource()
{
    check(SSEngine::IsInitialized() == true);
    check(DXRenderingThread::IsInRenderingThread() == true);
}

DXRenderResource::~DXRenderResource()
{
	check(SSEngine::IsInitialized() == true);
	check(DXRenderingThread::IsInRenderingThread() == true);
}