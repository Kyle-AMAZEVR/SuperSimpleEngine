
#include "SSRendererModulePCH.h"
#include "SSDX11VertexBuffer.h"


SSDX11VertexBuffer::SSDX11VertexBuffer() 
	: mBufferDescription{}
{
}

void* SSDX11VertexBuffer::GetBufferPointer()
{
	return (void*) mpBuffer.Get();
}
void* const* SSDX11VertexBuffer::GetBufferPointerRef()
{
	return (void* const*)mpBuffer.GetAddressOf();
}


SSDX11VertexBuffer::~SSDX11VertexBuffer()
{
	if(mpBuffer)
	{
		mpBuffer->Release();
		mpBuffer = nullptr;
	}
}