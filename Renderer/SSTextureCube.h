#pragma once

#include "SSTexture2DBase.h"
#include <vector>

class DX11RENDERER_API SSTextureCube : public SSTexture2DBase
{
public:
	
	bool LoadFromDDSFile(std::wstring filename);
	static std::shared_ptr<SSTextureCube> CreateFromDDSFile(std::wstring filename);
	
protected:
	
	bool bSRGB = false;
};
