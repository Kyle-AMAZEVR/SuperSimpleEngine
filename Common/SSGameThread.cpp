
#include "SSCommon.h"
#include "SSGameThread.h"

bool SSGameThread::IsInGameThread()
{
	if (mGameThreadId== GetCurrentThreadId())
	{
		return true;
	}
	else
	{
		return false;
	}
}


DWORD SSGameThread::mGameThreadId = 0;