
#include "Core.h"
#include "SSCustomRenderThreadObject.h"
#include "SSSharedBufferCache.h"

SSRenderTargetCube::SSRenderTargetCube()
{
	mVB = SSSharedBufferCache::Get().GetCubeVertexBuffer();
	mIB = SSSharedBufferCache::Get().GetCubeIndexBuffer();
}


