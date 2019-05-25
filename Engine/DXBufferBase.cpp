
#include "Core.h"
#include "DXBufferBase.h"
#include "DXEngine.h"

DXConstantBuffer::DXConstantBuffer()
{
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;

    DXEngine::Get().GetDevice()->CreateConstantBuffer();
}