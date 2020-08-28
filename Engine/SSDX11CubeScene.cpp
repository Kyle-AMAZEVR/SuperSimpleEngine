
#include "Core.h"
#include "SSCube.h"
#include "SSSphere.h"
#include "SSDX11CubeScene.h"

void SSDX11CubeScene::InitializeScene()
{
	mCube = new SSCube();
	mCube->SetPositionX(10);
}


void SSDX11CubeScene::DestroyScene()
{
}

void SSDX11CubeScene::DrawScene(float deltaSec)
{
	
}
