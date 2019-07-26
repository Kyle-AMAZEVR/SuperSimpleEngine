
#include "Core.h"
#include "SSCube.h"
#include "DXVertexTypes.h"
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"
#include <vector>

SSCube::SSCube()
{
	if (bIsInitialized == false)
	{
		InternalCreate();
		bIsInitialized = true;
	}
}

void SSCube::Draw(ID3D11DeviceContext* deviceContext)
{
	mYaw += 0.001f;

	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mCubeVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mCubeVB->GetBufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mCubeIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mCubeIB->GetIndexCount(), 0, 0);
}

void SSCube::InternalCreate()
{
	std::vector<VT_PositionNormalTexcoord> vertexArray =
	{	
		// Create vertex buffer		
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1), XMFLOAT3(1,0,0), XMFLOAT2(0,0)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f,1), XMFLOAT3(1,0,0),  XMFLOAT2(0,1)},
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f,1), XMFLOAT3(1,0,0),   XMFLOAT2(1,1)  },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f,1),  XMFLOAT3(1,0,0),   XMFLOAT2(1,0)  },
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f,1),  XMFLOAT3(1,0,0),   XMFLOAT2(0,0)  },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f,1),  XMFLOAT3(1,0,0),   XMFLOAT2(0,1)  },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f,1),  XMFLOAT3(1,0,0),   XMFLOAT2(1,1)  },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f,1),  XMFLOAT3(1,0,0),   XMFLOAT2(1,0)  },
	};

	std::vector<UINT> indexArray
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};

	

	mCubeVB = new SSVertexBuffer();
	mCubeVB->SetVertexBufferData(vertexArray);

	mCubeIB = new SSIndexBuffer();
	mCubeIB->SetIndexBufferData(indexArray);	
}
// 
SSIndexBuffer* SSCube::mCubeIB = nullptr;
SSVertexBuffer* SSCube::mCubeVB = nullptr;
bool SSCube::bIsInitialized = false;