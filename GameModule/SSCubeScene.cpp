
#include "SSCube.h"
#include "SSSphere.h"
#include "SSCubeScene.h"

void SSCubeScene::InitializeScene()
{
	mCube = std::make_shared<SSCube>();
	mCube->SetPositionX(10);

	mSphere = std::make_shared<SSSphere>(static_cast<UINT>(20),static_cast<UINT>(20), 10.0f);
	mSphere->SetPositionX(10);
	mSphere->SetPositionY( 10);

	mPBRSphere = std::make_shared<SSPBRSphere>();
	mPBRSphere->SetPositionY(-10);
}

void SSCubeScene::DestroyScene()
{

}
