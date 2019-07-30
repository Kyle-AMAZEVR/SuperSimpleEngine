#pragma once

#include "DirectXMath.h"

struct Transform
{
	DirectX::XMFLOAT4X4 Model;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
};

struct MVP
{
	DirectX::XMMATRIX ModelViewProj;
};