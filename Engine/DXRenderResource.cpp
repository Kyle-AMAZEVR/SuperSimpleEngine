
#include "Core.h"
#include "DXRenderResource.h"
#include "DXEngine.h"
#include "DXRenderingThread.h"

DXRenderResource::DXRenderResource()
{
    check(DXEngine::IsInitialized() == true);
    check(DXRenderingThread::IsInRenderingThread() == true);
}