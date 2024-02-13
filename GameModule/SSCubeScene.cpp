
#include "SSCube.h"
#include "SSSphere.h"
#include "SSCubeScene.h"
#include "SSGLTF.h"

void SSCubeScene::InitializeScene()
{
	mCube = std::make_shared<SSCube>();
	mCube->SetPositionX(30);

	mSphere = std::make_shared<SSSphere>(static_cast<UINT>(20),static_cast<UINT>(20), 10.0f);
	mSphere->SetPositionX(30);
	mSphere->SetPositionY( 10);

	mPBRSphere = std::make_shared<SSPBRSphere>();
	mPBRSphere->SetPositionX(40);
	mPBRSphere->SetPositionY(-10);

	SSGLTF_V2::LoadGLTFFile("./Resource/GLTF/DamagedHelmet/glTF/DamagedHelmet.gltf");
}

void SSCubeScene::DestroyScene()
{
	mCube.reset();
	mSphere.reset();
	mPBRSphere.reset();
}


void SSCubeScene::Tick(float DeltaSeconds)
{
	Elapsed += DeltaSeconds;

	if (Elapsed > 3)
	{
		Elapsed = 0;

		mCube.reset();

		mCube = std::make_shared<SSCube>();

		mCube->SetPositionX(static_cast<float>(std::rand() % 30 + 30));
	}
}