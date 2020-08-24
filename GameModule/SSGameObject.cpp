
#include "SSCommon.h"
#include "SSGameObject.h"
#include "SSMeshRenderData.h"
#include "SSGameObjectManager.h"

using namespace DirectX;

SSGameObject::SSGameObject()
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);

	mObjectId = SSGameObjectManager::Get().IssueObjectId();
	
	SSGameObjectManager::Get().AddGameObject(this);	
}

SSGameObject::~SSGameObject()
{
	SSGameObjectManager::Get().RemoveGameObject(mObjectId);
	mObjectId = 0;
}

SSMeshRenderData SSGameObject::mSharedRenderData;

const SSMeshRenderData& SSGameObject::GetRenderData()
{
	if(mHasPerInstanceRenderData)
	{
		return mPerInstanceRenderData;
	}
	else
	{
		return mSharedRenderData;
	}
}

XMMATRIX SSGameObject::GetModelTransform()
{
	return XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z) *  XMMatrixRotationY(mYaw) * XMMatrixScaling(mScale.x, mScale.y, mScale.z);
}

void SSGameObject::SetPositionX(float x)
{
	mPosition.x = x;
}

void SSGameObject::SetPositionY(float y)
{
	mPosition.y = y;
}

void SSGameObject::SetPositionZ(float z)
{
	mPosition.z = z;
}

void SSGameObject::SetScaleX(float x)
{
	mScale.x = x;
}

void SSGameObject::SetScaleY(float y)
{
	mScale.y = y;
}

void SSGameObject::SetScaleZ(float z)
{
	mScale.z = z;
}

void SSGameObject::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}
void SSGameObject::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
}
