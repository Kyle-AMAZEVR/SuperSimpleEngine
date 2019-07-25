
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
	mYaw += 0.1f;

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
		// Left
		{DirectX::XMFLOAT4(-1,   1, 1, 1), DirectX::XMFLOAT3(-1,0,0),DirectX::XMFLOAT2(0,0)},
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

	for (auto i = 0; i < vertexArray.size(); ++i)
	{
		indexArray.push_back(i);
	}

	mCubeVB = std::make_shared<SSVertexBuffer>();
	mCubeVB->SetVertexBufferData(vertexArray);

	mCubeIB = std::make_shared<SSIndexBuffer>();
	mCubeIB->SetIndexBufferData(indexArray);	
}
// 
std::shared_ptr<SSIndexBuffer> SSCube::mCubeIB;
std::shared_ptr<SSVertexBuffer> SSCube::mCubeVB;
bool SSCube::bIsInitialized = false;