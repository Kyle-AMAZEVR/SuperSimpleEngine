

#pragma once

#include <vector>
#include "SSBufferBase.h"
#include "SSEngine.h"

class SSDX11VertexBuffer : public SSBufferBase
{
public:
    SSDX11VertexBuffer(){}
    
    template<class T>
    void SetVertexBufferData(const std::vector<T>& vertexData);
	UINT GetStride() const;
	UINT GetVertexCount() const;

protected:
    template<class T>
    void InternalCreateVertexBuffer(const std::vector<T>& vertexData);
	UINT mStride = 0;
	UINT mVertexCount = 0;
};

template<class T>
void SSDX11VertexBuffer::SetVertexBufferData(const std::vector<T>& vertexData)
{
    if(mpBuffer == nullptr)
    {
        InternalCreateVertexBuffer(vertexData);
    }
}

template<class T>
void SSDX11VertexBuffer::InternalCreateVertexBuffer(const std::vector<T>& vertexData)
{
    mBufferDescription.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = static_cast<UINT>(sizeof(T) * vertexData.size());

    D3D11_SUBRESOURCE_DATA vertexSubresourceData;

    vertexSubresourceData.pSysMem = &vertexData[0];
	vertexSubresourceData.SysMemPitch = 0;
	vertexSubresourceData.SysMemSlicePitch = 0;

    HR(SSDX11Engine::Get().GetDevice()->CreateBuffer(&mBufferDescription, &vertexSubresourceData, &mpBuffer));

	mStride = sizeof(T);
	mVertexCount = static_cast<UINT>(vertexData.size());
}
