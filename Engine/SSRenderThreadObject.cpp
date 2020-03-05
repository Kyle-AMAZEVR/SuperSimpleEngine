
#include "Core.h"
#include "SSRenderThreadObject.h"
#include "SSEngine.h"
#include "SSRenderingThread.h"

SSRenderThreadObject::SSRenderThreadObject()
{    
    check(SSRenderingThread::IsInRenderingThread() == true);
}

SSRenderThreadObject::~SSRenderThreadObject()
{	
	check(SSRenderingThread::IsInRenderingThread() == true);
}