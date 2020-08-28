#pragma once

#include "SSGameScene.h"


class ENGINE_API SSDX11CubeScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
	void DrawScene(float deltaSec) override;
protected:
	class SSCube* mCube = nullptr;
	class SSSphere* mSphere = nullptr;
};

