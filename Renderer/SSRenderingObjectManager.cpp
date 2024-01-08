#include "SSRenderingObjectManager.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"
#include "SSGameObjectManager.h"

SSRenderingObjectManager::SSRenderingObjectManager()
{
	InitializeCriticalSection(&mCriticalSection);
}

void SSRenderingObjectManager::Initialize()
{
}

void SSRenderingObjectManager::Shutdown()
{
	DeleteAllObjects();
}

// called from rendering thread
void SSRenderingObjectManager::UpdateObjects()
{
	EnterCriticalSection(&mCriticalSection);

	auto& GameObjects = SSGameObjectManager::Get().GetGameObjectMap();
	
	for (auto& [k, v] : GameObjects)
	{
		if (!mRenderingObjectMap.contains(k))
		{
			mRenderingObjectMap[k] = new SSRenderingObject(v);
		}
	}
	
	std::vector<int> ObjectsToDelete;

	for (auto& [k, v] : mRenderingObjectMap)
	{
		if (!GameObjects.contains(k))
		{
			ObjectsToDelete.push_back(k);
		}
	}

	for (int Id : ObjectsToDelete)
	{
		SSRenderingObject* Object = mRenderingObjectMap[Id];
		
		delete Object;

		mRenderingObjectMap.erase(Id);
	}

	LeaveCriticalSection(&mCriticalSection);
}

void SSRenderingObjectManager::DeleteAllObjects()
{
	EnterCriticalSection(&mCriticalSection);

	for (auto& [k, v] : mRenderingObjectMap)
	{
		delete mRenderingObjectMap[k];
	}

	mRenderingObjectMap.clear();

	LeaveCriticalSection(&mCriticalSection);
}

// called from game thread
void SSRenderingObjectManager::SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap)
{
	EnterCriticalSection(&mCriticalSection);
	mPendingObjectMap = objectMap;
	LeaveCriticalSection(&mCriticalSection);
}



