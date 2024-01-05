
#include "SSRendererModulePCH.h"
#include "SSScreenBlit.h"
#include "SSVertexTypes.h"
#include <vector>
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSSharedBufferCache.h"
#include "SSSharedRenderData.h"
#include "SSRenderCommand.h"

SSScreenBlit::SSScreenBlit()
{
	mVisible = false;

	SSSimpleVertexData VertexData = SSSharedRenderData::Get().GetScreenBlitVertexData();

	mVertexData.Count = VertexData.Count;
	mVertexData.Stride = VertexData.Stride;
	mVertexData.DataPtr = VertexData.DataPtr;

	mVertexData.PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mVertexData.bHasIndexData = true;
	mVertexData.IndexData = SSSharedRenderData::Get().GetScreenBlitIndexData();

	mScreenVB = SSSharedBufferCache::Get().GetScreenBlitVertexBuffer();
	mScreenIB = SSSharedBufferCache::Get().GetScreenIndexBuffer();

}

void SSScreenBlit::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mScreenVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, (ID3D11Buffer* const*) mScreenVB->GetBufferPointerRef(), &stride, &offset);
	
	deviceContext->IASetIndexBuffer((ID3D11Buffer*) mScreenIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mScreenIB->GetIndexCount(), 0, 0);
}
