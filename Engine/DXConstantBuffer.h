
#pragma once

#include "DXBufferBase.h"

template<class TBufferType>
class DXConstantBuffer : public DXBufferBase
{
public:
    DXConstantBuffer();
    
    void Write(const TBufferType& data);

protected:

};


template<class TBufferType>
DXConstantBuffer<TBufferType>::DXConstantBuffer()
{
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = sizeof(TBufferType);

    HR(DXEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, &mpBuffer));
}


template<class TBufferType>
void DXConstantBuffer<TBufferType>::Write(const TBufferType& data)
{
    check(mpBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR(DXEngine::Get().GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, &mappedResource));
    
    memcpy(mappedResource.pData, &data, sizeof(data));

    HR(DXEngine::Get().GetDeviceContext()->Unmap(mpBuffer, 0));
}