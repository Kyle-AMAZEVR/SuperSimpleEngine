
#include "Core.h"
#include "SSScreenBlit.h"
#include "DXVertexTypes.h"
#include <vector>
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"


SSScreenBlit::SSScreenBlit()
{
	if (bIsInitialized == false)
	{
		InternalCreate();
		bIsInitialized = true;
	}
}

void SSScreenBlit::InternalCreate()
{
	std::vector<VT_PositionTexcoord> vertexArray
	{
		{XMFLOAT4(-1,1,0,1), XMFLOAT2(0,0)},
		{XMFLOAT4( 1,1,0,1), XMFLOAT2(1,0)},
		{XMFLOAT4( 1,-1,0,1), XMFLOAT2(1,1)},
		{XMFLOAT4(-1,-1,0,1), XMFLOAT2(0,1)},
	};

	std::vector<UINT> indexArray
	{
		0,1,2,
		0,2,3,
	};

	mScreenVB = new SSVertexBuffer();
	mScreenIB = new SSIndexBuffer();

	mScreenVB->SetVertexBufferData(vertexArray);
	mScreenIB->SetIndexBufferData(indexArray);
}

void SSScreenBlit::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mScreenVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mScreenVB->GetBufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mScreenIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mScreenIB->GetIndexCount(), 0, 0);
}

SSIndexBuffer* SSScreenBlit::mScreenIB = nullptr;
SSVertexBuffer* SSScreenBlit::mScreenVB = nullptr;
bool SSScreenBlit::bIsInitialized = false;