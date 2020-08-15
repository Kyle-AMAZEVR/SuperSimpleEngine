
#include "SSCommon.h"
#include "SSGameObject.h"
#include "SSGameObjectManager.h"


SSGameObjectManager& SSGameObjectManager::Get()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSGameObjectManager();
	}
	return *mInstance;
}

SSGameObjectManager* SSGameObjectManager::GetPtr()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSGameObjectManager();
	}
	return mInstance;
}

SSGameObjectManager* SSGameObjectManager::mInstance = nullptr;

void SSGameObjectManager::AddGameObject(SSGameObject* object)
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

void SSGameObjectManager::RemoveGameObject(UINT id)
{
	check(mSceneObjectMap.find(id) != mSceneObjectMap.end());
	mSceneObjectMap.erase(id);
}
