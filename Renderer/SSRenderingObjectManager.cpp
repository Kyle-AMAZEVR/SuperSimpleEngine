
#include "SSRendererModulePCH.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"


void SSRenderingObjectManager::Tick(float deltaSeconds)
{
	
}

void SSRenderingObjectManager::Tick(const std::map<UINT, SSObjectBase*>& objectMap)
{
	const auto& gameObjectMap = objectMap;
	//
	for (auto [k, v] : gameObjectMap)
	{
		if (mRenderingObjectMap.count(k) == 0 && v->IsVisible() == true)
		{
			mRenderingObjectMap[k] = new SSRenderingObject(v);
		}
	}
	//
	for (auto [k, v] : mRenderingObjectMap)
	{
		if (gameObjectMap.count(k) == 0)
		{

		}
	}
}
