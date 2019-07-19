#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSSceneObject : public DXRenderResource
{
public:
	virtual XMMATRIX GetModelTransform();

protected:
	XMFLOAT3 mPosition;
	XMFLOAT3 mScale;

};