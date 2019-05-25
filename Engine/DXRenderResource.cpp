
#include "Core.h"
#include "DXRenderResource.h"
#include "DXEngine.h"
#include "DXRenderingThread.h"

DXRenderResource::DXRenderResource()
{
    check(DXEngine::Get().GetDevice() != nullptr);
    check(DXRenderingThread::IsInRenderingThread() == true);
}