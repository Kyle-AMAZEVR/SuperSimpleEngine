#pragma once

#include "SSTexture2DBase.h"

class ENGINE_API SSTextureCube : public SSTexture2DBase
{
public:

	bool LoadFromDDSFile(std::wstring filename);

protected:
	UINT mMipLevels = 1;
	bool bSRGB = true;
};
