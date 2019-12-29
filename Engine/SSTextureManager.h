#pragma once

#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSName.h"
#include <map>

class ENGINE_API SSTextureManager : public Singleton<SSTextureManager>, public SSManagerBase
{
public:
	
	std::shared_ptr<class SSTexture2D> LoadTexture2D(ID3D11DeviceContext* deviceContext, SSName texturepath, bool bSRGB = false);

private:
	std::map<SSName, std::shared_ptr<class SSTexture2D>> m2DTextureMap;
	std::map<SSName, std::shared_ptr<class SSTextureCube>> mCubeTextureMap;
};
