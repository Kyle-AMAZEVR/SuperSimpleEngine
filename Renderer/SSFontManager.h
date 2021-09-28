#pragma once
#include "Singleton.h"



class DX11RENDERER_API SSFontCharacter
{
public:
	

	unsigned long mCharCode;
	std::vector<XMFLOAT2> mPoints;
};

class DX11RENDERER_API SSFontManager : public Singleton<SSFontManager>
{
public:
	bool IsInitialized() const { return bInitialized; }
	void Initialize(int dpiX, int dpiY);
	SSFontCharacter& GetCharacterInfo(unsigned long charCode);


protected:
	bool bInitialized = false;
	std::map<unsigned long, SSFontCharacter> mFontMap;

};