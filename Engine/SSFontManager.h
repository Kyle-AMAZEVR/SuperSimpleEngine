#pragma once
#include "Singleton.h"
#include <functional>
#include <map>
#include <vector>
#include "ft2build.h"
#include FT_FREETYPE_H


class ENGINE_API SSFontCharacter
{
public:
	
	

	FT_ULong mCharCode;
	std::vector<XMFLOAT2> mPoints;
};

class ENGINE_API SSFontManager : public Singleton<SSFontManager>
{
public:
	bool IsInitialized() const { return bInitialized; }
	void Initialize(int dpiX, int dpiY);

protected:
	bool bInitialized = false;

	
};