
#include "SSRenderCommand.h"
#include "SSShader.h"
#include "SSDX11Texture2D.h"
#include "SSDX11RenderTarget.h"

void SSRenderCmdSetVS::Execute(ID3D11DeviceContext * inDeviceContext)
{
	inDeviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
}

void SSRenderCmdSetPS::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetShader(mPS->GetShader(), nullptr, 0);
}

SSRenderCmdSetVSTexture::SSRenderCmdSetVSTexture(SSDX11VertexShader* inVS, SSDX11Texture2D * inTex, unsigned int slotIndex)
	:mVS(inVS), mTex(inTex),mSlotIndex(slotIndex)
{

}

void SSRenderCmdSetVSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->VSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdSetPSTexture::SSRenderCmdSetPSTexture(class SSDX11PixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex)
	: mPS(inPS), mTex(inTex), mSlotIndex(slotIndex)
{
}

void SSRenderCmdSetPSTexture::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetShaderResources(mSlotIndex, 1, mTex->GetShaderResourceViewRef());
}

SSRenderCmdSetVSCBuffer::SSRenderCmdSetVSCBuffer(SSDX11VertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot)	
	: mVS(inVS), mBuffer(inBuffer),mSlotIndex(slot)
{
}

void SSRenderCmdSetVSCBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->VSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*) mBuffer->GetBufferPointer());
}

SSRenderCmdSetPSCBuffer::SSRenderCmdSetPSCBuffer(SSDX11PixelShader* inPS, class SSDX11Buffer* inBuffer, unsigned int slot)
	: mPS(inPS), mBuffer(inBuffer), mSlotIndex(slot)
{
}

void SSRenderCmdSetPSCBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	inDeviceContext->PSSetConstantBuffers(mSlotIndex, 1, (ID3D11Buffer* const*)mBuffer->GetBufferPointer());
}

SSRenderCmdCopyCBuffer::SSRenderCmdCopyCBuffer(SSDX11Buffer* ptrBuffer)
	: mBuffer(ptrBuffer)
{
}

void SSRenderCmdCopyCBuffer::Execute(ID3D11DeviceContext* inDeviceContext)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HR(inDeviceContext->Map(mBuffer->GetDX11BufferPointer(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource));
	memcpy_s(mappedResource.pData, mBuffer->GetBufferSize(),mBuffer->GetBufferDataPtr(), mBuffer->GetBufferSize());
	inDeviceContext->Unmap(mBuffer->GetDX11BufferPointer(), 0);
}


SSRenderCmdSetRenderTarget::SSRenderCmdSetRenderTarget(class SSDX11RenderTarget* inRT)
	: mRenderTarget(inRT)
{
}

void SSRenderCmdSetRenderTarget::Execute(ID3D11DeviceContext* inDeviceContext)
{	
	if (mRenderTarget)
	{
		ID3D11RenderTargetView* viewArray[SSDX11RenderTarget::MAX_COUNT]{ nullptr };
		ID3D11DepthStencilView* depthStencilView{ nullptr };

		const unsigned int nCount = mRenderTarget->GetCount();

		for (auto i = 0u; i < nCount; ++i)
		{
			viewArray[i] = mRenderTarget->GetRenderTargetView(nCount);
		}

		if (mRenderTarget->IsDepthExist())
		{
			depthStencilView = mRenderTarget->GetDepthStencilView();
		}

		inDeviceContext->OMSetRenderTargets(nCount, viewArray, depthStencilView); 
	}
}


SSRenderCmdDrawIndexed::SSRenderCmdDrawIndexed(SSDX11IndexBuffer * inBuffer)
	: mIndexBuffer(inBuffer)
{
}

void SSRenderCmdDrawIndexed::Execute(ID3D11DeviceContext* inDeviceContext)
{
	
}