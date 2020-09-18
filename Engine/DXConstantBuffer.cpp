
#include "Core.h"
#include "DXConstantBuffer.h"
#include "SSDX11Engine.h"
#include "SSShader.h"

// trigger compile


SSGenericConstantBuffer::SSGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index)
{
    mBufferIndex = index;

    check(constantBuffer != nullptr);

    D3D11_SHADER_BUFFER_DESC bufferDesc;
    constantBuffer->GetDesc(&bufferDesc);

	mBufferSize = bufferDesc.Size;
    
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
    mBufferData = new BYTE[mBufferSize] {0};

    // now create constant buffer
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = mBufferSize;

    HR(SSDX11Engine::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, mpBuffer.ReleaseAndGetAddressOf()));
}

SSGenericConstantBuffer::~SSGenericConstantBuffer()
{
    if(mBufferData != nullptr)
    {
        delete [] mBufferData;
        mBufferData = nullptr;
    }
}

void SSGenericConstantBuffer::StoreBufferProxyData(const SSConstantBufferProxy& data)
{   
    check(mpBuffer != nullptr);
    
    check(data.GetBufferSize() == mBufferSize);

    memcpy_s(mBufferData, mBufferSize, data.GetData(), mBufferSize);
}

void SSGenericConstantBuffer::SubmitDataToDevice(ID3D11DeviceContext* deviceContext)
{ 	
	check(deviceContext);

    D3D11_MAPPED_SUBRESOURCE mappedResource;
	deviceContext->Map(mpBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy_s(mappedResource.pData, mBufferSize, mBufferData, mBufferSize);
	deviceContext->Unmap(mpBuffer.Get(), 0);
}
