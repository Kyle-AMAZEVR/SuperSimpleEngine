

#pragma once

#include <vector>
#include "DXBufferBase.h"

class DXVertexBuffer : public DXBufferBase
{
public:
    DXVertexBuffer(){}
    
    template<class T>
    void SetVertexData(const std::vector<T>& vertexData);

protected:
    template<class T>
    void InternalCreateVertexBuffer(const std::vector<T>& vertexData);

    
};

template<class T>
void DXVertexBuffer::SetVertexData(const std::vector<T>& vertexData)
{
    if(mpBuffer == nullptr)
    {
        InternalCreateVertexBuffer(vertexData);
    }
}

template<class T>
void DXVertexBuffer::InternalCreateVertexBuffer(const std::vector<T>& vertexData)
{
    mBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    mBufferDescription.CPUAccessFlags = 0;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = sizeof(T) * vertexData.size();

    D3D11_SUBRESOURCE_DATA vertexSubresourceData;

    vertexSubresourceData.pSysMem = &vertexData[0];
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

    HR(DXEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, &vertexSubresourceData, &mpBuffer));
}
