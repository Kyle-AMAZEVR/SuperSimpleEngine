
#pragma once

#include "SSDX11Buffer.h"
#include <vector>
#include <string>
#include "SSConstantBufferProxy.h"
#include "SSDX11Renderer.h"



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

