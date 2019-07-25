#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSSceneObject : public DXRenderResource
{
public:
	SSSceneObject();

	virtual XMMATRIX GetModelTransform();

	virtual void SetPosition(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	virtual void SetPositionX(float x);
	virtual void SetPositionY(float y);
	virtual void SetPositionZ(float z);


	virtual void SetScaleX(float x);
	virtual void SetScaleY(float y);
	virtual void SetScaleZ(float z);

protected:
	friend class SSSceneObjectManager;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mScale;
	UINT mObjectId = 0;
};