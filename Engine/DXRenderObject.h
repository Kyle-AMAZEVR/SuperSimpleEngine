#pragma once

#include "DXRenderResource.h"
#include "DirectXMath.h"

using namespace DirectX;

class ENGINE_API DXRenderObject : public DXRenderResource
{
public:

	virtual XMMATRIX GetModelTransform();

protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mScale;
};