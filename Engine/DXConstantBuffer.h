
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

template<unsigned int bufferSize>
class DXGenericConstantBuffer : public DXBufferBase
{
public:
    DXGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer)
    {
        static_assert(bufferSize > 0);
        check(constantBuffer != nullptr);       
        
        D3D11_SHADER_BUFFER_DESC bufferDesc;
        constantBuffer->GetDesc(&bufferDesc);
        
        // 
        for(unsigned int i = 0; i < bufferDesc.Variables; ++i)
        {
            ID3D11ShaderReflectionVariable* variableReflection = constantBuffer->GetVariableByIndex(i);
            D3D11_SHADER_VARIABLE_DESC variableDesc;
            variableReflection->GetDesc(&variableDesc);
            
            ID3D11ShaderReflectionType* variableType = variableReflection->GetType();
            D3D11_SHADER_TYPE_DESC typeDesc;
            variableType->GetDesc(&typeDesc);

            VariableInConstantBufferInfo info 
            {
                static_cast<USHORT>(variableDesc.StartOffset),
                static_cast<USHORT>(variableDesc.Size),
                static_cast<BYTE>(i),
                variableDesc.Name
            };

            mVariableInfoArray.push_back(info);
        }        
    }

    

protected:
    unsigned char mBufferData[bufferSize] = { 0 };
    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;
};