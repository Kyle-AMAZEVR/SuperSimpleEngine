
#include "SSRendererModulePCH.h"
#include "SSGameObject.h"
#include "SSGameObjectManager.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"

void SSRenderingObjectManager::Tick(float deltaSeconds)
{
	const auto& gameObjectMap = SSGameObjectManager::Get().GetGameObjectMap();
	//
	for(auto [k,v] : gameObjectMap)
	{
		if(mRenderingObjectMap.count(k) == 0 && v->IsVisible() == true)
		{
			mRenderingObjectMap[k] = new SSRenderingObject(v);
		}
	}
	//
	for(auto [k,v] : mRenderingObjectMap)
	{
		if(gameObjectMap.count(k) == 0)
		{
			
		}
	}
}
