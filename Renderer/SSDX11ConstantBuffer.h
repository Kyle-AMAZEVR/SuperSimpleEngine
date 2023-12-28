
#pragma once

#include "SSDX11Buffer.h"
#include <vector>
#include <string>
#include "SSConatantBufferData.h"
#include "SSDX11Renderer.h"


/////////////////////////////////////

struct VariableInConstantBufferInfo
{
    USHORT StartOffset;
    USHORT Size;
    BYTE Index;
    std::string Name;
};

class SSDX11ConstantBuffer : public SSDX11Buffer
{
public:
    SSDX11ConstantBuffer(ID3D11ShaderReflectionConstantBuffer* constantBuffer, UINT index);

    SSDX11ConstantBuffer(ID3D11Buffer* InBuffer, UINT InSlotIndex, UINT InSize, std::string InName);

    virtual ~SSDX11ConstantBuffer() override;
    
    template<class T>
    void SetBufferData(ID3D11DeviceContext* deviceContext, const T& value);

    void StoreBufferProxyData(const SSConatantBufferData& data);

	template<class T>
	void StoreBufferData(const T& value);

    virtual BYTE* GetBufferDataPtr() override { return mBufferData; }

protected:
    BYTE* mBufferData = nullptr;
    std::vector<VariableInConstantBufferInfo> mVariableInfoArray;    

	friend class SSDrawCommand;
	friend class SSMaterial;

    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext) override;
};


template<class T>
void SSDX11ConstantBuffer::SetBufferData(ID3D11DeviceContext* deviceContext, const T& value)
{
	StoreBufferData<T>(value);

    SubmitDataToDevice(deviceContext);
}

template<class T>
void SSDX11ConstantBuffer::StoreBufferData(const T& value)
{
	check(mpBuffer != nullptr);

	UINT valueSize = sizeof(value);
	check(valueSize == mBufferSize);
	
	memcpy_s(mBufferData, mBufferSize, &value, mBufferSize);
}

