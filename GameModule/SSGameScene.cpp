
#include "SSGameObject.h"
#include "SSGameScene.h"

void SSGameScene::Tick(float deltaSeconds)
{
	
}

void SSGameScene::InitializeScene()
{
	
}

void SSGameScene::DestroyScene()
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
	
}

