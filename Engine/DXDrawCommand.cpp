

#include "Core.h"
#include "DXEngine.h"
#include "DXDrawCommand.h"
#include "DXVertexBuffer.h"


void SSDrawCommand::Do()
{
	ID3D11DeviceContext* deviceContext = DXEngine::Get().GetDeviceContext();

	assert(mVS != nullptr);
	assert(mIB != nullptr);
	
	
	deviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);

	deviceContext->IASetVertexBuffers(0, 1, mVB->GetBufferPointerRef(), nullptr, nullptr);

}