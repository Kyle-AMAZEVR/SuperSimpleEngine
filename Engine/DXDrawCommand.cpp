

#include "Core.h"
#include "SSEngine.h"
#include "DXDrawCommand.h"
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSSceneObject.h"

SSDrawCommand::SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSSceneObject> object)
	: mpVS(vs), mpPS(ps), mObject(object)
{

}

void SSDrawCommand::Do()
{
	check(mpVS != nullptr);
	check(mpPS != nullptr);

	ID3D11DeviceContext* deviceContext = SSEngine::Get().GetDeviceContext();

	// @ set input layout
	deviceContext->IASetInputLayout(mpVS->GetInputLayout());

	// @ set vertex, pixel shader
	deviceContext->VSSetShader(mpVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mpPS->GetShader(), nullptr, 0);

	// @ set vertex shader constant buffer
	for (auto& kvp : mVertexShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice();
	}

	// @ set pixel shader constant buffer
	for (auto& kvp : mPixelShaderConstantBufferMap)
	{
		kvp.second->SubmitDataToDevice();
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

	// @ draw
	mObject->Draw(deviceContext);
}


void SSDrawCommand::SetPSTexture(std::string name, SSTexture2D* texture)
{
	mPixelShaderTextureMap[name] = texture;
}


void SSDrawCommand::SetVSTexture(std::string name, SSTexture2D* texture)
{
	mVertexShaderTextureMap[name] = texture;
}