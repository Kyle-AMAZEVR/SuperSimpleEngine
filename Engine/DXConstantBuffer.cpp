
#include "Core.h"
#include "DXConstantBuffer.h"
#include "DXEngine.h"
#include "DXShader.h"

// trigger compile


DXGenericConstantBuffer::DXGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer)
{
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
    auto bufferSize = mVariableInfoArray[bufferDesc.Variables - 1].StartOffset +  mVariableInfoArray[bufferDesc.Variables - 1].Size;
    mBufferData = new BYTE[bufferSize] {0};
}

DXGenericConstantBuffer::~DXGenericConstantBuffer()
{
    if(mBufferData != nullptr)
    {
        delete [] mBufferData;
        mBufferData = nullptr;
    }
}