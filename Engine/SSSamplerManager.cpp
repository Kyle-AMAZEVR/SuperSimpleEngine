#include "Core.h"
#include "SSSamplerManager.h"
#include "SSEngine.h"

void SSSamplerManager::Initialize()
{
	CreateDefaultSampler();
}

void SSSamplerManager::CreateDefaultSampler()
{
	D3D11_SAMPLER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_SAMPLER_DESC));
	desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	desc.MipLODBias = 0;
	desc.MaxAnisotropy = 1;
	desc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	desc.MinLOD = 0;
	desc.MaxLOD = 0;
	desc.Filter = D3D11_FILTER_ANISOTROPIC;

	auto* dxDevice = SSEngine::Get().GetDevice();

	HR(dxDevice->CreateSamplerState(&desc, &mDefaultSamplerState));
}