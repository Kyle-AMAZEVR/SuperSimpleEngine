
#include "Core.h"
#include "SSGameObjectManager.h"
#include "SSRenderingObjectManager.h"

void SSRenderingObjectManager::Tick(float deltaSeconds)
{
	const auto& gameObjectMap = SSGameObjectManager::Get().GetGameObjectMap();
	for(auto [k,v] : gameObjectMap)
	{
		
	}
}
