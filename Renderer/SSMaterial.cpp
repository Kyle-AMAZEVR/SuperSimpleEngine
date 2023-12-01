
#include "SSRendererModulePCH.h"
#include "SSMaterial.h"
#include "SSShader.h"
#include "SSDX11Renderer.h"

SSMaterial::SSMaterial(std::shared_ptr<SSDX11VertexShader> vs, std::shared_ptr<SSDX11PixelShader> ps)
	: mVS(vs), mPS(ps)
{
	mVertexShaderConstantBufferMap = mVS->GetConstantBufferMap();
	mPixelShaderConstantBufferMap = mPS->GetConstantBufferMap();
}


void SSMaterial::SetCurrent()
{
	ID3D11DeviceContext* deviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();

	// @ set input layout
	deviceContext->IASetInputLayout(mVS->GetInputLayout());

	// @ for now ony
	deviceContext->IASetPrimitiveTopology(mPrimitiveType);

	// @ set vertex, pixel shader

	deviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);

}


void SSMaterial::SetPSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
{	
	mPixelShaderTextureMap[name] = texture;
	mPS->SetTexture(deviceContext, name, texture);
}


void SSMaterial::SetVSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
{	
	mVertexShaderTextureMap[name] = texture;
	mVS->SetTexture(deviceContext, name, texture);
}

void SSMaterial::SetPSSampler(std::string name, ID3D11SamplerState* sampler)
{	
	mPS->SetSampler(name, sampler);
}

void SSMaterial::ReleaseCurrent()
{
	for (auto& [k,_] : mPixelShaderTextureMap)
	{
		mPS->SetTextureAsNull(k);
	}
}

void SSMaterial::SetVSConstantBufferProxyData(ID3D11DeviceContext* deviceContext, SSName name, const SSConatantBufferData& data)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferProxyData(data);
		mVertexShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mVertexShaderConstantBufferMap[name]->GetBufferIndex();

		check(deviceContext != nullptr);

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*) mVertexShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}

void SSMaterial::SetPSConstantBufferProxyData(ID3D11DeviceContext* deviceContext, SSName name, const SSConatantBufferData& data)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->StoreBufferProxyData(data);
		mPixelShaderConstantBufferMap[name]->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = mPixelShaderConstantBufferMap[name]->GetBufferIndex();

		check(deviceContext != nullptr);

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*)mPixelShaderConstantBufferMap[name]->GetBufferPointerRef());
	}
}