
#include "Core.h"
#include "SSCube.h"
#include "DXVertexTypes.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSDX11Renderer.h"
#include <vector>

SSCube::SSCube()
{	
}

void SSCube::PrepareRendering()
{
	check(SSRenderingThread::IsInRenderingThread());
	InternalCreate();	
	mRenderingReady = true;
}


void SSCube::Draw(SSDX11Renderer* renderer)
{
	check(renderer != nullptr);
	Draw(renderer->GetImmediateDeviceContext());
}

void SSCube::Draw(ID3D11DeviceContext* deviceContext)
{

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mCubeVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mCubeVB->GetBufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mCubeIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mCubeIB->GetIndexCount(), 0, 0);
}

void SSCube::InternalCreate()
{
	std::vector<VT_PositionNormalTexcoord> vertexArray =
	{	
		// front
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1), XMFLOAT3(0,0,-1), XMFLOAT2(0,0)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),  XMFLOAT2(0,1)},
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),   XMFLOAT2(1,1)  },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f,1),  XMFLOAT3(0,0,-1),   XMFLOAT2(1,0)  },

		// back
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,0)  },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,1)  },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,1)  },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,0)  },

		// top
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,0)},
		{ XMFLOAT4( 1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,0)},
		{ XMFLOAT4( 1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,1)},

		
		// bottom
		{ XMFLOAT4(-1.0f,-1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,0)},
		{ XMFLOAT4(1.0f, -1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,0)},
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,1)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,1)},

		// left
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,0)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,0)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,1)},
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,1)},

		// right
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,0)},
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,0)},
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,1)},
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,1)},
	};

	std::vector<UINT> indexArray
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// top
		8,9,11,
		11,9,10,

		// bottom
		14,13,12,
		15,14,12,

		// left
		16,17,18,
		16,18,19,

		// right
		20,21,22,
		20,22,23,
	};

	

	mCubeVB = new SSDX11VertexBuffer();
	mCubeVB->SetVertexBufferData(vertexArray);

	mCubeIB = new SSIndexBuffer();
	mCubeIB->SetIndexBufferData(indexArray);	
}
// 
SSIndexBuffer* SSCube::mCubeIB = nullptr;
SSDX11VertexBuffer* SSCube::mCubeVB = nullptr;
bool SSCube::bIsInitialized = false;