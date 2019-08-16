
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
	ID3D11DeviceContext* deviceContext = SSEngine::Get().GetDeviceContext();

	// @ set input layout
	deviceContext->IASetInputLayout(mpVS->GetInputLayout());

	// @ for now ony
	deviceContext->IASetPrimitiveTopology(mPrimitiveType);

	// @ set vertex, pixel shader

	deviceContext->VSSetShader(mpVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mpPS->GetShader(), nullptr, 0);

}


void SSMaterial::SetPSTexture(std::string name, SSTexture2DBase* texture)
{
	check(mpPS != nullptr);
	mpPS->SetTexture(name, texture);
}


void SSMaterial::SetVSTexture(std::string name, SSTexture2DBase* texture)
{
	check(mpVS != nullptr);
	mpVS->SetTexture(name, texture);
}

void SSMaterial::SetPSSampler(std::string name, ID3D11SamplerState* sampler)
{
	check(mpPS != nullptr);
	mpPS->SetSampler(name, sampler);
}