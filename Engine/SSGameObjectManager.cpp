
#include "Core.h"
#include "SSGameObject.h"
#include "SSGameObjectManager.h"

void SSGameObjectManager::AddGameObject(SSGameObject* object)
{
	check(object != nullptr);
	check(object->GetId() != 0);
	mGameObjectMap[object->GetId()] = object;
}

void SSGameObjectManager::Tick(float delta)
{
	for (auto& item : mGameObjectMap)
	{
		item.second->Tick(delta);
	}
}

void SSGameObjectManager::RemoveGameObject(UINT id)
{
	check(mGameObjectMap.find(id) != mGameObjectMap.end());
	mGameObjectMap.erase(id);
}
