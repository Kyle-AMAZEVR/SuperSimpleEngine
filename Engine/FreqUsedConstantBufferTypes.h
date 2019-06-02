#pragma once

#include "DirectXMath.h"

struct Transform
{
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;
};