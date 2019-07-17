#pragma once
#include "Windows.h"

class ScopedLock
{
public:
	ScopedLock();
	~ScopedLock();
protected:
	CRITICAL_SECTION mCriticalSection;
};
