#pragma once

#include "SSGameScene.h"


class DX11RENDERER_API SSDX11CubeScene : public SSGameScene
{
public:
	void InitializeScene() override;
	void DestroyScene() override;
protected:
	class std::shared_ptr<class SSCube> mCube = nullptr;
	class std::shared_ptr<class SSSphere> mSphere = nullptr;
	class std::shared_ptr<class SSPBRSphere> mPBRSphere = nullptr;
};

