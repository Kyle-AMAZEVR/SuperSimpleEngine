#pragma once

#include "SSGameScene.h"


class ENGINE_API SSDX11CubeScene : public SSGameScene
{
public:
	void InitializeScene() override;	
protected:
	class SSCube* mCube = nullptr;
};

