

#include "Core.h"
#include "SSEngine.h"
#include "SSDrawCommand.h"
#include "SSSceneObject.h"
#include "SSSamplerManager.h"
#include "SSMaterial.h"


SSChangeRenderTargetCmd::SSChangeRenderTargetCmd(IRenderTarget* renderTarget)
	: mRenderTarget(renderTarget)
{	
}

void SSChangeRenderTargetCmd::Do()
{
	mRenderTarget->SetCurrentRenderTarget();	
}


SSDrawCommand::SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSSceneObject> object)
	: mpVS(vs), mpPS(ps), mObject(object)
{
	mVertexShaderConstantBufferMap = mpVS->GetConstantBufferMap();
	mPixelShaderConstantBufferMap = mpPS->GetConstantBufferMap();
}

void SSDrawCommand::DoWithMaterial()
{
	check(mMaterial != nullptr);

	ID3D11DeviceContext* deviceContext = SSEngine::Get().GetImmediateDeviceContext();

	mMaterial->SetCurrent();

	// @ set vertex shader constant buffer
	for (auto& kvp : mVertexShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice();

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader constant buffer
	for (auto& kvp : mPixelShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice();

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader texture 
	for (auto& kvp : mVertexShaderTextureMap)
	{
		mMaterial->SetVSTexture(kvp.first, kvp.second);
	}
	
	// @set verte shader texture
	for (auto& kvp : mPixelShaderTextureMap)
	{
		mMaterial->SetPSTexture(kvp.first, kvp.second);
	}
	
	// @ draw
	mObject->Draw(deviceContext, mMaterial);

	for (auto& kvp : mPixelShaderTextureMap)
	{
		mpPS->SetTextureAsNull(kvp.first);
	}
	
}

void SSDrawCommand::Do()
{
	check(mpVS != nullptr);
	check(mpPS != nullptr);

	//
	if(mPreDrawJob != nullptr)
	{
		mPreDrawJob();
	}

	ID3D11DeviceContext* deviceContext = SSEngine::Get().GetImmediateDeviceContext();

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
		kvp.second->SubmitDataToDevice();

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->VSSetConstantBuffers(bufferIndex, 1, kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader constant buffer
	for (auto& kvp : mPixelShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice();

		UINT bufferIndex = kvp.second->GetBufferIndex();

		deviceContext->PSSetConstantBuffers(bufferIndex, 1, kvp.second->GetBufferPointerRef());
	}

	// @ set pixel shader texture 
	for (auto& kvp : mPixelShaderTextureMap)
	{
		mpPS->SetTexture(kvp.first, kvp.second);
	}

	// @ set vertex shader texture 
	for (auto& kvp : mVertexShaderTextureMap)
	{
		mpVS->SetTexture(kvp.first, kvp.second);
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

	if(mPostDrawJob != nullptr)
	{
		mPostDrawJob();
	}
}

void SSDrawCommand::SetPostDrawJob(std::function<void()> job)
{
	mPostDrawJob = std::move(job);
}

void SSDrawCommand::SetPreDrawJob(std::function<void()> job)
{
	mPreDrawJob = std::move(job);
}




void SSDrawCommand::SetPSTexture(std::string name, SSTexture2DBase* texture)
{
	mPixelShaderTextureMap[name] = texture;
}


void SSDrawCommand::SetVSTexture(std::string name, SSTexture2DBase* texture)
{
	mVertexShaderTextureMap[name] = texture;
}