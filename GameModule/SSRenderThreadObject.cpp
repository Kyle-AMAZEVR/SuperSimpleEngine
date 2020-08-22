#include "SSCommon.h"
#include "SSRenderThreadObject.h"
#include "SSRenderingThread.h"

SSRenderThreadObject::SSRenderThreadObject()
{    
    check(SSRenderingThread::IsInRenderingThread() == true);
}

SSRenderThreadObject::~SSRenderThreadObject()
{	
	check(SSRenderingThread::IsInRenderingThread() == true);
}