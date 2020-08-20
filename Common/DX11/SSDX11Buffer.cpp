
#include "SSCommon.h"
#include "SSDX11Buffer.h"

void SSDX11Buffer::Destroy()
{
	mpBuffer.Reset();
}

ID3D11Buffer* const* SSDX11Buffer::GetDX11BufferPointerRef()
{
	return mpBuffer.GetAddressOf();
}

ID3D11Buffer* SSDX11Buffer::GetDX11BufferPointer()
{
	return mpBuffer.Get();
}
