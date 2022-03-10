

#include "SSRenderingObjectManager.h"

SSRenderingObjectManager::SSRenderingObjectManager()
{
	InitializeCriticalSection(&mCriticalSection);
}

// called from rendering thread
void SSRenderingObjectManager::UpdateObjects()
{
	EnterCriticalSection(&mCriticalSection);
	
	for (auto& [k, v] : mPendingObjectMap)
	{
		if (mRenderingObjectMap.count(k) == 0 && v->IsVisible() == true)
		{
			mRenderingObjectMap[k] = new SSRenderingObject(v);
		}
	}
	
	LeaveCriticalSection(&mCriticalSection);
}

// called from game thread
void SSRenderingObjectManager::SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap)
{
	EnterCriticalSection(&mCriticalSection);
	mPendingObjectMap = objectMap;
	LeaveCriticalSection(&mCriticalSection);
}