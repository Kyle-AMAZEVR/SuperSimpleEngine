
#include "SSDX11Buffer.h"


void* const* SSDX11Buffer::GetBufferPointerRef()
{
	return (void* const*) mpBuffer.GetAddressOf();
}

void* SSDX11Buffer::GetBufferPointer()
{
	return mpBuffer.Get();
}
