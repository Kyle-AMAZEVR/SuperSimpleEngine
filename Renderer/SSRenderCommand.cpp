
#include "SSRenderCommand.h"
#include "SSShader.h"
#include "SSDX11Texture2D.h"

void SSRenderCmdSetVS::Execute(ID3D11DeviceContext * inDeviceContext)
{
	inDeviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
}

void SSRenderCmdSetPS::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);
}

SSRenderCmdSetVSTexture::SSRenderCmdSetVSTexture(SSVertexShader* inVS, SSDX11Texture2D * inTex, unsigned int slotIndex)
	:mVS(inVS), mTex(inTex),mSlotIndex(slotIndex)
{

}

void SSRenderCmdSetVSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->VSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdSetPSTexture::SSRenderCmdSetPSTexture(class SSPixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex)
	: mPS(inPS), mTex(inTex), mSlotIndex(slotIndex)
{
}

void SSRenderCmdSetPSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdSetVSCBuffer::SSRenderCmdSetVSCBuffer(SSVertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot)	
	: mVS(inVS), mBuffer(inBuffer),mSlotIndex(slot)
{
}

void SSRenderCmdSetVSCBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->VSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*) mBuffer->GetBufferPointer());
}

SSRenderCmdSetPSCBuffer::SSRenderCmdSetPSCBuffer(SSPixelShader* inPS, class SSDX11Buffer* inBuffer, unsigned int slot)
	: mPS(inPS), mBuffer(inBuffer), mSlotIndex(slot)
{
}

void SSRenderCmdSetPSCBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*)mBuffer->GetBufferPointer());
}