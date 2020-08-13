
#include "Core.h"
#include "SSSceneObject.h"
#include "SSSceneObjectManager.h"

void SSSceneObjectManager::AddSceneObject(SSSceneObject* object)
{
	check(object != nullptr);
	check(object->GetId() != 0);
	mSceneObjectMap[object->GetId()] = object;
}

void SSSceneObjectManager::Tick(float delta)
{
	for (auto& item : mSceneObjectMap)
	{
		item.second->Tick(delta);
	}
}

void SSSceneObjectManager::RemoveSceneObject(UINT id)
{
	check(mSceneObjectMap.find(id) != mSceneObjectMap.end());
	mSceneObjectMap.erase(id);
}
