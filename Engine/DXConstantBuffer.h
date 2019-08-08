
#pragma once

#include "SSBufferBase.h"
#include <vector>
#include <string>

template<class TBufferType>
class SSTypedConstantBuffer : public SSBufferBase
{
public:
    SSTypedConstantBuffer();
    
    void Write(const TBufferType& data);

protected:

};


template<class TBufferType>
SSTypedConstantBuffer<TBufferType>::SSTypedConstantBuffer()
{
    mBufferDescription.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    mBufferDescription.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    mBufferDescription.Usage = D3D11_USAGE_DYNAMIC;
    mBufferDescription.MiscFlags = 0;
    mBufferDescription.StructureByteStride = 0;
    mBufferDescription.ByteWidth = sizeof(TBufferType);

    HR(SSEngine::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, &mpBuffer));
}


template<class TBufferType>
void SSTypedConstantBuffer<TBufferType>::Write(const TBufferType& data)
{
    check(mpBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR(SSEngine::Get().GetDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, &mappedResource));
    
    memcpy(mappedResource.pData, &data, sizeof(data));

    HR(SSEngine::Get().GetDeviceContext()->Unmap(mpBuffer, 0));   
    
}


/////////////////////////////////////

struct VariableInConstantBufferInfo
{
    USHORT StartOffset;
    USHORT Size;
    BYTE Index;
    std::string Name;
};

class SSGenericConstantBuffer : public SSBufferBase
{
public:
    SSGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index);
    virtual ~SSGenericConstantBuffer() override;
    
    template<class T>
    void SetBufferSubData(std::string name, const T& value);

    template<class T>
    void SetBufferData(const T& value);

	template<class T>
	void StoreBufferData(const T& value);

protected:
    BYTE* mBufferData = nullptr;
    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;

	friend class SSDrawCommand;

    virtual void SubmitDataToDevice() override;
};

template<class T>
void SSGenericConstantBuffer::SetBufferSubData(std::string name , const T& value)
{
    for(auto& iter : mVariableInfoArray)
    {
        if(iter->Name == name)
        {
            check(iter->Size == sizeof(T));                       
			memcpy_s(mBufferData + iter->StartOffset, iter->Size, &value, iter->Size);
            break;
        }
    }

    SubmitDataToDevice();
}

template<class T>
void SSGenericConstantBuffer::SetBufferData(const T& value)
{
	StoreBufferData<T>(value);

    SubmitDataToDevice();
}

template<class T>
void SSGenericConstantBuffer::StoreBufferData(const T& value)
{
	check(mpBuffer != nullptr);

	check(sizeof(value) == mBufferSize);

	memcpy_s(mBufferData, mBufferSize, &value, mBufferSize);
}