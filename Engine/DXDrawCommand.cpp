

#include "Core.h"
#include "DXEngine.h"
#include "DXDrawCommand.h"
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"

void SSDrawCommand::Do()
{
	ID3D11DeviceContext* deviceContext = DXEngine::Get().GetDeviceContext();

	assert(mVS != nullptr);
	assert(mIB != nullptr);
	
	// @ set input layout
	deviceContext->IASetInputLayout(mVS->GetInputLayout());
	
	// @ set vertex, pixel shader
	deviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
	deviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);
	
	// @ set vertex, index buffer, primitive topology
	deviceContext->IASetVertexBuffers(0, 1, &mVB->GetBufferPointerRef(), nullptr, nullptr);
	deviceContext->IASetIndexBuffer(mIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(mIB->GetPrimitiveType());

	// @ set vertex shader constant buffers
	// deviceContext->VSSetConstantBuffers();

	// @ set 

	deviceContext->DrawIndexed(mIB->GetIndexCount(), 0, 0);
}