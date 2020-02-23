#pragma once

#include "SSRenderThreadObject.h"
#include "SSName.h"

using namespace std;

class SSVertexShader;
class SSPixelShader;
class SSTexture2DBase;

class ENGINE_API SSMaterial : public SSRenderThreadObject
{
public:
	SSMaterial(SSVertexShader* vs, SSPixelShader* ps);
	//SSMaterial(SSVertexShader* vs, SSPixelShader* ps);

	virtual void SetCurrent();
	virtual void ReleaseCurrent();

	template<class T>
	void SetVSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value);
	template<class T>
	void SetPSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value);

	template<class T>
	void SetVSConstantBufferDataChecked(ID3D11DeviceContext* deviceContext, SSName name, const T& value);

	void SetPSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture);
	void SetVSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture);

	virtual void SetPSSampler(std::string name, ID3D11SamplerState* sampler);

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }	

protected:
	class SSVertexShader* mpVS;
	class SSPixelShader* mpPS;

	//
	std::map<SSName, class SSGenericConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<SSName, class SSGenericConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSTexture2DBase*> mPixelShaderTextureMap;
	std::map<std::string, class SSTexture2DBase*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
};


template<class T>
void SSMaterial::SetVSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);
		mVertexShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);
		
		UINT bufferIndex = mVertexShaderConstantBufferMap[name]->GetBufferIndex();

		check(deviceContext != nullptr);

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, mVertexShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}

template<class T>
void SSMaterial::SetVSConstantBufferDataChecked(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);
		mVertexShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mVertexShaderConstantBufferMap[name]->GetBufferIndex();

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, mVertexShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
	else
	{
		check(false);
	}
}

template<class T>
void SSMaterial::SetPSConstantBufferData(ID3D11DeviceContext* deviceContext, SSName name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->StoreBufferData(value);
		mPixelShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mPixelShaderConstantBufferMap[name]->GetBufferIndex();

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, mPixelShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}
