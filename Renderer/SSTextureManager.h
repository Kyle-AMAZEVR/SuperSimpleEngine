#pragma once

#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSName.h"
#include <map>

class DX11RENDERER_API SSTextureManager : public Singleton<SSTextureManager>, public SSManagerBase
{
public:
	
	std::shared_ptr<class SSDX11Texture2D> LoadTexture2D(ID3D11DeviceContext* deviceContext, SSName texturepath, bool bSRGB = false);

private:
	std::map<SSName, std::shared_ptr<class SSDX11Texture2D>> m2DTextureMap;
	std::map<SSName, std::shared_ptr<class SSTextureCube>> mCubeTextureMap;
};
