
#include "SSGameObject.h"
#include "SSGameScene.h"

void SSGameScene::Tick(float deltaSeconds)
{
	
}

void SSGameScene::InitializeScene()
{
	
}

void SSGameScene::AddGameObject(class SSGameObject* gameObject)
{
	if(gameObject)
	{
		mSceneObjectMap[gameObject->GetId()] = gameObject;
		gameObject->OnAddedScene();
	}
}


void SSGameScene::DrawScene(float deltaSec)
{
	for(auto& kvp : mSceneObjectMap)
	{
		if(kvp.second != nullptr)
		{
			if(false == kvp.second->IsRenderingReady())
			{
				kvp.second->PrepareRendering();
			}

			
		}
	}
}

