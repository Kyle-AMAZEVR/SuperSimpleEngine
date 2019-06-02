
#pragma once

#include "DXBufferBase.h"
#include <vector>
#include <string>

template<class TBufferType>
class DXTypedConstantBuffer : public DXBufferBase
{
public:
    DXTypedConstantBuffer();
    
    void Write(const TBufferType& data);

protected:

};


template<class TBufferType>
DXTypedConstantBuffer<TBufferType>::DXTypedConstantBuffer()
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
void DXTypedConstantBuffer<TBufferType>::Write(const TBufferType& data)
{
    check(mpBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR(DXEngine::Get().GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, &mappedResource));
    
    memcpy(mappedResource.pData, &data, sizeof(data));

    HR(DXEngine::Get().GetDeviceContext()->Unmap(mpBuffer, 0));   
    
}


/////////////////////////////////////

struct VariableInConstantBufferInfo
{
    USHORT StartOffset;
    USHORT Size;
    BYTE Index;
    std::string Name;
};

class DXGenericConstantBuffer : public DXBufferBase
{
public:
    DXGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer);    
    
    template<class T>
    void SetConstantBufferData(std::string name, const T& value);

protected:
    BYTE* mBufferData = nullptr;
    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;
};

template<class T>
void SetConstantBufferData(std::string name , const T& value)
{
	
}