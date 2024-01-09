#include "SSRendererModulePCH.h"
#include "SSDX11Renderer.h"
#include "SSDX11IndexBuffer.h"

SSDX11IndexBuffer::SSDX11IndexBuffer(ID3D11Buffer* ptrBuffer, unsigned int nIndexCount)
    : mpBuffer(ptrBuffer)
{
    mIndexCount = nIndexCount;
    ZeroMemory(&mBufferDescription, sizeof(mBufferDescription));
}

void* SSDX11IndexBuffer::GetBufferPointer()
{
    return mpBuffer.Get();
}

void* const* SSDX11IndexBuffer::GetBufferPointerRef()
{
    return (void* const*)mpBuffer.GetAddressOf();
}

SSDX11IndexBuffer::~SSDX11IndexBuffer()
{
    if (mpBuffer)
    {
        mpBuffer.Reset();
    }
}

void SSDX11IndexBuffer::SetIndexBufferData(const std::vector<UINT> indexData, D3D_PRIMITIVE_TOPOLOGY ePrimitiveType )
{
	mPrimitiveType = ePrimitiveType;
	mIndexCount = static_cast<UINT>(indexData.size());

    mBufferDescription.BindFlags = D3D11_BIND_INDEX_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE ;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = static_cast<UINT>(sizeof(unsigned int) * indexData.size());

    D3D11_SUBRESOURCE_DATA indexSubresourceData;

    indexSubresourceData.pSysMem = &indexData[0];
	indexSubresourceData.SysMemPitch = 0;
	indexSubresourceData.SysMemSlicePitch = 0;

    HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, &indexSubresourceData, &mpBuffer));
}