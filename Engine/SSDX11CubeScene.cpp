
#include "Core.h"
#include "SSCube.h"
#include "SSSphere.h"
#include "SSDX11CubeScene.h"

void SSDX11CubeScene::InitializeScene()
{
	mCube = std::make_shared<SSCube>();
	mCube->SetPositionX(10);

	mSphere = std::make_shared<SSSphere>(20,20,10);
	mSphere->SetPositionX(10);
	mSphere->SetPositionY( 10);
}

void SSDX11CubeScene::DestroyScene()
{

}
