
#include "Core.h"
#include "SSCube.h"
#include "DXVertexTypes.h"
#include "DXVertexBuffer.h"
#include "DXIndexBuffer.h"
#include <vector>

SSCube::SSCube()
{
}

void SSCube::InternalCreate()
{
	std::vector<VT_PositionNormalTexcoord> vertexArray =
	{
		// Left
		{DirectX::XMFLOAT4(-1,   1, 1, 1), DirectX::XMFLOAT3(-1,0,0), DirectX::XMFLOAT2(0,0)},
		{DirectX::XMFLOAT4(-1,   1,-1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(1,0)},
		{DirectX::XMFLOAT4(-1,  -1, 1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(0,1)},
				
		{DirectX::XMFLOAT4(-1,  -1, 1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(0,1)},
		{DirectX::XMFLOAT4(-1,   1,-1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(1,0)},
		{DirectX::XMFLOAT4(-1,  -1,-1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(1,1)},

		// Right
		{DirectX::XMFLOAT4(1,  1,-1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(0,0)},
		{DirectX::XMFLOAT4(1,  1, 1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(1,0)},
		{DirectX::XMFLOAT4(1, -1, 1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(1,1)},

		{DirectX::XMFLOAT4(1,  1,-1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(0,0)},
		{DirectX::XMFLOAT4(1, -1, 1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(1,1)},
		{DirectX::XMFLOAT4(1, -1,-1, 1), DirectX::XMFLOAT3(1,0,0), DirectX::XMFLOAT2(0,1)},
		

		// front
		{DirectX::XMFLOAT4(-1,   1, -1, 1), DirectX::XMFLOAT3(0,0,-1), DirectX::XMFLOAT2(0,0)},
		{DirectX::XMFLOAT4( 1,   1, -1, 1), DirectX::XMFLOAT3(0,0,-1),DirectX::XMFLOAT2(1,0)},
		{DirectX::XMFLOAT4( 1,  -1, -1, 1), DirectX::XMFLOAT3(0,0,-1),DirectX::XMFLOAT2(1,1)},
		

		{DirectX::XMFLOAT4(1,   1, -1, 1), DirectX::XMFLOAT3(0,0,-1),DirectX::XMFLOAT2(1,0)},
		{DirectX::XMFLOAT4(1,  -1, -1, 1), DirectX::XMFLOAT3(0,0,-1),DirectX::XMFLOAT2(1,1)},
		{DirectX::XMFLOAT4(-1, -1, -1, 1), DirectX::XMFLOAT3(0,0,-1),DirectX::XMFLOAT2(0,1)},
	};

	std::vector<UINT> indexArray;

	mCubeVB = std::make_unique<DXVertexBuffer>();
	mCubeVB->SetVertexBufferData(vertexArray);

	mCubeIB = std::make_unique<DXIndexBuffer>();
	mCubeIB->SetIndexBufferData(indexArray);
}
// 
std::unique_ptr<DXIndexBuffer> SSCube::mCubeIB;
std::unique_ptr<DXVertexBuffer> SSCube::mCubeVB;
