#include "Core.h"
#include "DXEngine.h"
#include "DXIndexBuffer.h"


void DXIndexBuffer::SetIndexBufferData(const std::vector<UINT> indexData, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType )
{
	mPrimitiveType = ePrimitiveType;
	mIndexCount = indexData.size();

    mBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE ;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = sizeof(unsigned int) * indexData.size();

    D3D11_SUBRESOURCE_DATA indexSubresourceData;

    indexSubresourceData.pSysMem = &indexData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

    HR(DXEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, &indexSubresourceData, &mpBuffer));
}