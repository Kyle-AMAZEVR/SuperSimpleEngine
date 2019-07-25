#include "Core.h"
#include "SSSceneObject.h"
#include "SSSceneObjectManager.h"

SSSceneObject::SSSceneObject()
{
	mObjectId = SSSceneObjectManager::Get().IssueObjectId();
	SSSceneObjectManager::Get().AddSceneObject(this);

	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
}

SSSceneObject::~SSSceneObject()
{
	SSSceneObjectManager::Get().RemoveSceneObject(mObjectId);
}

XMMATRIX SSSceneObject::GetModelTransform()
{
	return XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z) * XMMatrixScaling(mScale.x, mScale.y, mScale.z);
}

void SSSceneObject::SetPositionX(float x)
{
	mPosition.x = x;
}

void SSSceneObject::SetPositionY(float y)
{
	mPosition.y = y;
}

void SSSceneObject::SetPositionZ(float z)
{
	mPosition.z = z;
}

void SSSceneObject::SetScaleX(float x)
{
	
}

void SSSceneObject::SetScaleY(float y)
{

}

void SSSceneObject::SetScaleZ(float z)
{

}

void SSSceneObject::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}
void SSSceneObject::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
}
