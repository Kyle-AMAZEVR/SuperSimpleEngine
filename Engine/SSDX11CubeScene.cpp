
#include "Core.h"
#include "SSCube.h"
#include "SSDX11CubeScene.h"

void SSDX11CubeScene::InitializeScene()
{
	mCube = new SSCube();
	this->AddGameObject(mCube);
}


void SSDX11CubeScene::DestroyScene()
{
}

void SSDX11CubeScene::DrawScene(float deltaSec)
{
	
}
