
#include "Core.h"
#include "SSMaterial.h"
#include "SSShader.h"
#include "SSEngine.h"

SSMaterial::SSMaterial(SSVertexShader* vs, SSPixelShader* ps)
	: mpVS(vs), mpPS(ps)
{	
	mVertexShaderConstantBufferMap = mpVS->GetConstantBufferMap();
	mPixelShaderConstantBufferMap = mpPS->GetConstantBufferMap();
}

void SSMaterial::SetCurrent()
{
	ID3D11DeviceContext* deviceContext = SSDX11Engine::Get().GetImmediateDeviceContext();

	// @ set input layout
	deviceContext->IASetInputLayout(mpVS->GetInputLayout());

	// @ for now ony
	deviceContext->IASetPrimitiveTopology(mPrimitiveType);

	// @ set vertex, pixel shader

	deviceContext->VSSetShader(mpVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mpPS->GetShader(), nullptr, 0);

}


void SSMaterial::SetPSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture)
{
	check(mpPS != nullptr);
	mPixelShaderTextureMap[name] = texture;
	mpPS->SetTexture(deviceContext, name, texture);
}


void SSMaterial::SetVSTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture)
{
	check(mpVS != nullptr);
	mVertexShaderTextureMap[name] = texture;
	mpVS->SetTexture(deviceContext, name, texture);
}

void SSMaterial::SetPSSampler(std::string name, ID3D11SamplerState* sampler)
{
	check(mpPS != nullptr);
	mpPS->SetSampler(name, sampler);
}

void SSMaterial::ReleaseCurrent()
{
	for (auto& kvp : mPixelShaderTextureMap)
	{
		mpPS->SetTextureAsNull(kvp.first);
	}

}