
#include "SSGameModule.h"
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
