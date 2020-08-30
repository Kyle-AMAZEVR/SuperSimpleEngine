#include "SSCommon.h"
#include "SSRenderThreadObject.h"
#include "SSRenderingThread.h"

using namespace DirectX;

SSRenderThreadObject::SSRenderThreadObject()
{    
    check(SSRenderingThread::IsInRenderingThread() == true);
}

SSRenderThreadObject::~SSRenderThreadObject()
{	
	check(SSRenderingThread::IsInRenderingThread() == true);
}
