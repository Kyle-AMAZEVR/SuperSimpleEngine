
#pragma once

#include "DXBufferBase.h"

class DXConstantBuffer : public DXBufferBase
{
public:
    DXConstantBuffer();
    
    template<class T>
    void Write(const T& data);

protected:

};

template<class T>
void DXConstantBuffer::Write(const T& data)
{
    check(mpBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR(DXEngine::Get().GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, &mappedResource));
    
    memcpy(mappedResource.pData, &data, sizeof(data));

    HR(DXEngine::Get().GetDeviceContext()->Unmap(mpBuffer, 0));
}