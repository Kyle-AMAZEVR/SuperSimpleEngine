

#include "ScopedLock.h"

ScopedLock::ScopedLock()
{
	InitializeCriticalSection(&mCriticalSection);
	EnterCriticalSection(&mCriticalSection);
}

ScopedLock::~ScopedLock()
{
	LeaveCriticalSection(&mCriticalSection);
}

