

#include "SSRendererModulePCH.h"
#include "SSDX11Renderer.h"
#include "SSDrawCommand.h"
#include "SSSamplerManager.h"
#include "SSMaterial.h"
#include "SSRenderTargetBase.h"
#include "SSDX11RenderTarget.h"
#include "SSShader.h"

SSChangeRenderTargetCmd::SSChangeRenderTargetCmd(IRenderTarget* renderTarget)
	: mRenderTarget(renderTarget)
{	
}

void SSChangeRenderTargetCmd::Do(SSDX11Device* device)
{
	//mRenderTarget->SetCurrentRenderTarget(device);	
}


SSDrawCommand::SSDrawCommand(std::shared_ptr<SSDX11VertexShader> vs, std::shared_ptr<SSDX11PixelShader> ps, std::shared_ptr<SSObjectBase> object)
	: mpVS(vs), mpPS(ps), mObject(object)
{
	mVertexShaderConstantBufferMap = mpVS->GetConstantBufferMap();
	mPixelShaderConstantBufferMap = mpPS->GetConstantBufferMap();
}

void SSDrawCommand::Do(SSDX11Device* device)
{
	check(mpVS != nullptr);
	check(mpPS != nullptr);

	ID3D11DeviceContext* deviceContext = device->GetDeviceContext();

	// @ set input layout
	deviceContext->IASetInputLayout(mpVS->GetInputLayout());

	// @ for now ony
	deviceContext->IASetPrimitiveTopology(mPrimitiveType);

	// @ set vertex, pixel shader
	deviceContext->VSSetShader(mpVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mpPS->GetShader(), nullptr, 0);

	// @ set vertex shader constant buffer
	for (auto& kvp : mVertexShaderConstantBufferMap)
	{		
		kvp.second->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*) kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader constant buffer
	for (auto& kvp : mPixelShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice(deviceContext);

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, (ID3D11Buffer* const*)kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader texture 
	for (auto& kvp : mPixelShaderTextureMap)
	{
		mpPS->SetTexture(deviceContext, kvp.first, kvp.second);
	}

	// @ set vertex shader texture 
	for (auto& kvp : mVertexShaderTextureMap)
	{
		mpVS->SetTexture(deviceContext, kvp.first, kvp.second);
	}

	// @ set pixel shader sampler
	for (auto& samplerName : mpPS->GetSamplerNames())
	{
		ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();
		mpPS->SetSampler(samplerName, sampler);
	}	

	// @ draw
	mObject->Draw(deviceContext);

	for (auto& kvp : mPixelShaderTextureMap)
	{
		mpPS->SetTextureAsNull(kvp.first);
	}
}


void SSDrawCommand::SetPSTexture(std::string name, SSDX11Texture2D* texture)
{
	mPixelShaderTextureMap[name] = texture;
}


void SSDrawCommand::SetVSTexture(std::string name, SSDX11Texture2D* texture)
{
	mVertexShaderTextureMap[name] = texture;
}