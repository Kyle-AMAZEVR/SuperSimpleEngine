

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

	// @
	

	mObject->Draw(deviceContext);
}