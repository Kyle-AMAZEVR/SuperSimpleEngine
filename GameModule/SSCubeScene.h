#pragma once

#include "SSGameScene.h"


class GAMEMODULE_API SSCubeScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
protected:
	class std::shared_ptr<class SSCube> mCube = nullptr;
	class std::shared_ptr<class SSSphere> mSphere = nullptr;
	class std::shared_ptr<class SSPBRSphere> mPBRSphere = nullptr;
};

