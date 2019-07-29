#pragma once

#include "DXRenderResource.h"

class ENGINE_API SSSceneObject : public DXRenderResource
{
public:
	SSSceneObject();
	virtual ~SSSceneObject();

	virtual void Draw(ID3D11DeviceContext* deviceContext) {}

	virtual void Tick(float delta){}

	virtual XMMATRIX GetModelTransform();

	UINT GetId() const { return mObjectId; }

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

	float mYaw = 0;
	float mPitch = 0;
	float mRoll = 0;
	UINT mObjectId = 0;
};