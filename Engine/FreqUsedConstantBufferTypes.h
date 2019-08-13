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

 struct alignas(16) CbufferFloat
{
	float value;
};

 struct alignas(16) CbufferInvScreenSize
 {
	 XMFLOAT2 InvScreenSize;
 };