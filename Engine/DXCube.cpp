
#include "Core.h"
#include "DXCube.h"
#include "DXVertexTypes.h"
#include <vector>

DXCube::DXCube()
{

}

void DXCube::InternalCreate()
{
	std::vector<VT_PositionNormalTexcoord> VertexArray =
	{
		// Left
		{DirectX::XMFLOAT4(-1, -1, 0, 1), DirectX::XMFLOAT3(-1,0,0)},
		{DirectX::XMFLOAT4(-1,  1, 0, 1), DirectX::XMFLOAT3(-1,0,0)},
		{DirectX::XMFLOAT4(-1,  1, 0, 1), DirectX::XMFLOAT3(-1,0,0)},

		// Right
		{DirectX::XMFLOAT4(-1, -1, 0, 1), DirectX::XMFLOAT4(1,0,0,1)},
		{DirectX::XMFLOAT4(1,  1, 0, 1), DirectX::XMFLOAT4(1,0,0,1)},
		{DirectX::XMFLOAT4(1, -1, 0, 1), DirectX::XMFLOAT4(1,0,0,1)},
	};
}