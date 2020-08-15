
#include "Core.h"
#include "SSGameObject.h"
#include "SSGameObjectManager.h"

void SSGameObjectManager::AddSceneObject(SSGameObject* object)
{
	check(object != nullptr);
	check(object->GetId() != 0);
	mSceneObjectMap[object->GetId()] = object;
}

void SSGameObjectManager::Tick(float delta)
{
	for (auto& item : mSceneObjectMap)
	{
		item.second->Tick(delta);
	}
}

void SSGameObjectManager::RemoveSceneObject(UINT id)
{
	check(mSceneObjectMap.find(id) != mSceneObjectMap.end());
	mSceneObjectMap.erase(id);
}
