
#include "Core.h"
#include "DXConstantBuffer.h"
#include "DXEngine.h"
#include "DXShader.h"

// trigger compile


DXGenericConstantBuffer::DXGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index)
{
    mBufferIndex = index;

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

    // alloc
    mBufferSize = mVariableInfoArray[bufferDesc.Variables - 1].StartOffset +  mVariableInfoArray[bufferDesc.Variables - 1].Size;
    mBufferData = new BYTE[mBufferSize] {0};

    // now create constant buffer
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = mBufferSize;

    HR(DXEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, &mpBuffer));    
}

DXGenericConstantBuffer::~DXGenericConstantBuffer()
{
    if(mBufferData != nullptr)
    {
        delete [] mBufferData;
        mBufferData = nullptr;
    }
}

void DXGenericConstantBuffer::SubmitDataToDevice()
{ 
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HR(DXEngine::Get().GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE, 0, &mappedResource));    
    memcpy_s(mappedResource.pData, mBufferSize, mBufferData, mBufferSize);
    DXEngine::Get().GetDeviceContext()->Unmap(mpBuffer, 0);

    
}