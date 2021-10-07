
#include "SSRendererModulePCH.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"

void SSRenderingObjectManager::UpdateObjects()
{
	//
	for (auto& [k, v] : mPendingObjectMap)
	{
		if (mRenderingObjectMap.count(k) == 0 && v->IsVisible() == true)
		{
			mRenderingObjectMap[k] = new SSRenderingObject(v);
		}
	}

}

void SSRenderingObjectManager::SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap)
{
	mPendingObjectMap = objectMap;
}