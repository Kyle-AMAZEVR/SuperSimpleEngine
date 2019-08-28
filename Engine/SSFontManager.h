#pragma once
#include "Singleton.h"

class ENGINE_API SSFontManager : public Singleton<SSFontManager>
{
public:
	bool IsInitialized() const { return bInitialized; }
	void Initialize(int dpiX, int dpiY);

protected:
	bool bInitialized = false;
};