
#include "Core.h"
#include "DXRenderResource.h"
#include "DXEngine.h"

DXRenderResource::DXRenderResource()
{
    check(DXEngine::Get().GetDevice() != nullptr);
    check(DXRenderingThread::IsInRenderingThread() == true);
}