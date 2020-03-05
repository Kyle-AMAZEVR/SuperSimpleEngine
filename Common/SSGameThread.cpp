
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

void SSGameThread::Start(const DWORD GameThreadId)
{
	mGameThreadId = GameThreadId;
	mGameThreadDoneEventHandle = CreateEvent(nullptr, false, false, "GameThreadEventHandle");
}

void SSGameThread::Tick(float DeltaSeconds)
{
	// do game thread work
	
	// set event
	SetEvent(mGameThreadDoneEventHandle);
}


DWORD SSGameThread::mGameThreadId = 0;