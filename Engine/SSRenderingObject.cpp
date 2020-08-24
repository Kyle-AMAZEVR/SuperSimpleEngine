
#include "Core.h"
#include "SSGameObject.h"
#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"

SSRenderingObject::SSRenderingObject(SSGameObject* pGameObject)
	: mpGameObject(pGameObject)
{
	mRenderData = pGameObject->GetRenderData();
	
	mVertexBuffer = new SSDX11VertexBuffer();

	if(mRenderData.VertexType == EVertexType::VT_PNTT)
	{
		mVertexBuffer->SetVertexBufferData(mRenderData.PNTT_VertexData);
	}
	else if(mRenderData.VertexType == EVertexType::VT_PNT)
	{
		mVertexBuffer->SetVertexBufferData(mRenderData.PNT_VertexData);
	}		

	if(mRenderData.bHasIndexData)
	{
		mIndexBuffer = new SSIndexBuffer();
		mIndexBuffer->SetIndexBufferData(mRenderData.IndexData);
	}	
}


SSRenderingObject::~SSRenderingObject()
{
	if(mVertexBuffer)
	{		
		mVertexBuffer->Destroy();
		delete mVertexBuffer;
		mVertexBuffer = nullptr;
	}

	if(mIndexBuffer)
	{
		mIndexBuffer->Destroy();
		delete mIndexBuffer;
		mIndexBuffer = nullptr;
	}
}

void SSRenderingObject::Draw(ID3D11DeviceContext* deviceContext)
{	
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	auto stride = mVertexBuffer->GetStride();
	UINT offset = 0;

	// set vertex buffer
	deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetDX11BufferPointerRef(), &stride, &offset);

	// if have index buffer
	if(mRenderData.bHasIndexData)
	{
		// set indexbuffer and call draw indexed
		deviceContext->IASetIndexBuffer(mIndexBuffer->GetDX11BufferPointer(), DXGI_FORMAT_R32_UINT, 0);
		deviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);
	}
	else
	{
		deviceContext->Draw(mVertexBuffer->GetVertexCount(), 0);
	}
}
