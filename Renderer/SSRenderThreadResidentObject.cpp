
#include "SSRenderThreadResidentObject.h"
#include "SSCore.h"
#include "SSRenderingThread.h"

SSRenderThreadResidentObject::SSRenderThreadResidentObject()
{
    check(SSRenderingThread::IsInRenderingThread() == true);
}

SSRenderThreadResidentObject::~SSRenderThreadResidentObject()
{
    check(SSRenderingThread::IsInRenderingThread() == true);
}

