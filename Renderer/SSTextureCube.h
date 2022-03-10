#pragma once

#include "SSDX11Texture2D.h"
#include <vector>

class DX11RENDERER_API SSTextureCube : public SSDX11Texture2D
{
public:
	
	bool LoadFromDDSFile(std::wstring filename);
	static std::shared_ptr<SSTextureCube> CreateFromDDSFile(std::wstring filename);
	
protected:
	
	bool bSRGB = false;
};
