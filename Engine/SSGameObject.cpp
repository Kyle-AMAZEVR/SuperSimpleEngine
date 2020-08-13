#include "Core.h"
#include "SSGameObject.h"
#include "SSGameObjectManager.h"

SSGameObject::SSGameObject()
{
	mObjectId = SSGameObjectManager::Get().IssueObjectId();
	SSGameObjectManager::Get().AddGameObject(this);

	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
}

SSGameObject::~SSGameObject()
{
	SSGameObjectManager::Get().RemoveGameObject(mObjectId);
}

XMMATRIX SSGameObject::GetModelTransform()
{
	return mCachedModelTransform;
}

void SSGameObject::SetPositionX(float x)
{
	mPosition.x = x;
	bTransformDirty = true;
}

void SSGameObject::SetPositionY(float y)
{
	mPosition.y = y;
	bTransformDirty = true;
}

void SSGameObject::SetPositionZ(float z)
{
	mPosition.z = z;
	bTransformDirty = true;
}

void SSGameObject::SetScaleX(float x)
{
	mScale.x = x ;
	bTransformDirty = true;
}

void SSGameObject::SetScaleY(float y)
{
	mScale.y = y;
	bTransformDirty = true;
}

void SSGameObject::SetScaleZ(float z)
{
	mScale.z = z;
	bTransformDirty = true;
}

void SSGameObject::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
	bTransformDirty = true;
}
void SSGameObject::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
	bTransformDirty = true;
}

void SSGameObject::CacheTransform()
{
	mCachedModelTransform = XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z)* XMMatrixRotationY(mYaw)* XMMatrixScaling(mScale.x, mScale.y, mScale.z);	
}

void SSGameObject::GameThreadTick(float delta)
{
	if(bTransformDirty)
	{
		CacheTransform();
		bTransformDirty = false;
	}
}

void SSGameObject::RenderThreadTick(float delta)
{
}
