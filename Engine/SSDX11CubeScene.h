#pragma once

#include "SSGameScene.h"


class ENGINE_API SSDX11CubeScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
	void DrawScene(float deltaSec) override;
protected:
	class std::shared_ptr<class SSCube> mCube = nullptr;
	class std::shared_ptr<class SSSphere> mSphere = nullptr;
};

