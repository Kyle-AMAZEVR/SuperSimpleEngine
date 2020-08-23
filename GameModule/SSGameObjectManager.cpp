
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
