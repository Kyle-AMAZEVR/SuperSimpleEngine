

#include "Core.h"
#include "DXEngine.h"
#include "DXDrawCommand.h"
#include "DXVertexBuffer.h"
#include "DXIndexBuffer.h"

void SSDrawCommand::Do()
{
	ID3D11DeviceContext* deviceContext = DXEngine::Get().GetDeviceContext();

	assert(mVS != nullptr);
	assert(mIB != nullptr);	
	
	
	//
	deviceContext->IASetInputLayout(mVS->GetInputLayout());
	//
	deviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);
	//
	deviceContext->IASetVertexBuffers(0, 1, &mVB->GetBufferPointerRef(), nullptr, nullptr);
	deviceContext->IASetIndexBuffer(mIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);



}