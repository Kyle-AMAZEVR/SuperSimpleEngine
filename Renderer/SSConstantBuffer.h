
#pragma once

#include "SSDX11Buffer.h"
#include <vector>
#include <string>
#include "SSConstantBufferProxy.h"
#include "SSDX11Renderer.h"

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

    HR(SSDX11Renderer::Get().GetDevice()->CreateBuffer(&mBufferDescription, nullptr, &mpBuffer));
}


template<class TBufferType>
void SSTypedConstantBuffer<TBufferType>::Write(const TBufferType& data)
{
    check(mpBuffer != nullptr);

    D3D11_MAPPED_SUBRESOURCE mappedResource;

    HR(SSDX11Renderer::Get().GetImmediateDeviceContext()->Map(mpBuffer, 0, D3D11_MAP_WRITE_DISCARD, &mappedResource));
    
    memcpy(mappedResource.pData, &data, sizeof(data));

    HR(SSDX11Renderer::Get().GetImmediateDeviceContext()->Unmap(mpBuffer, 0));
    
}


/////////////////////////////////////

struct VariableInConstantBufferInfo
{
    USHORT StartOffset;
    USHORT Size;
    BYTE Index;
    std::string Name;
};

class DX11RENDERER_API SSGenericConstantBuffer : public SSDX11Buffer
{
public:
    SSGenericConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index);
    virtual ~SSGenericConstantBuffer() override;
    
    template<class T>
    void SetBufferData(ID3D11DeviceContext* deviceContext, const T& value);

    void StoreBufferProxyData(const SSConstantBufferProxy& data);

	template<class T>
	void StoreBufferData(const T& value);

protected:
    BYTE* mBufferData = nullptr;
    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;

	friend class SSDrawCommand;
	friend class SSMaterial;

    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext) override;
};


template<class T>
void SSGenericConstantBuffer::SetBufferData(ID3D11DeviceContext* deviceContext, const T& value)
{
	StoreBufferData<T>(value);

    SubmitDataToDevice(deviceContext);
}

template<class T>
void SSGenericConstantBuffer::StoreBufferData(const T& value)
{
	check(mpBuffer != nullptr);

	UINT valueSize = sizeof(value);
	check(valueSize == mBufferSize);
	
	memcpy_s(mBufferData, mBufferSize, &value, mBufferSize);
}

