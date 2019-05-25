
#include "Core.h"
#include "DXBufferBase.h"
#include "DXEngine.h"


DXBufferBase::~DXBufferBase()
{
    if(mpBuffer != nullptr)
    {
        mpBuffer->Release();
        mpBuffer = nullptr;
    }
}

DXConstantBuffer::DXConstantBuffer()
{
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;

    HR(DXEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, &mpBuffer));
}

