
#include "SSRendererModulePCH.h"
#include "SSDX11VertexBuffer.h"


UINT SSDX11VertexBuffer::GetStride() const
{	
	return mStride;
}

UINT SSDX11VertexBuffer::GetVertexCount() const
{
	return mVertexCount;
}