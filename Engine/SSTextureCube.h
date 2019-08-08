#pragma once

#include "SSTexture2DBase.h"
#include <vector>

class ENGINE_API SSTextureCube : public SSTexture2DBase
{
public:

	bool LoadFromDDSFiles(std::vector<std::wstring> sixFaceFiles);
	bool LoadFromHDRFile(std::wstring filename);
	bool LoadFromDDSFile(std::wstring filename);

protected:
	UINT mMipLevels = 1;
	bool bSRGB = true;
};
