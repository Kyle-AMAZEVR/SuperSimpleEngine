

#pragma once

#include <vector>
using namespace DirectX;

class GAMEMODULE_API SSTangentVectorGenerator
{

public:
	static std::vector<DirectX::XMFLOAT4> GenerateTangents(const std::vector<DirectX::XMFLOAT4>& tempVertexList,
		const std::vector<DirectX::XMFLOAT3>& tempNormalList,
		const std::vector<DirectX::XMFLOAT2>& tempTexCoordList);
};
