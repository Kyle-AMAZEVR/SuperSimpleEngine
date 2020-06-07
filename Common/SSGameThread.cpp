
#include "SSCommon.h"
#include "SSGameThread.h"
#include "Windows.h"
#include "SSGameWindow.h"

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

void SSGameThread::Start(DWORD gameThreadId)
{	
	mGameThreadId = gameThreadId;
	mGameThreadDoneEventHandle = CreateEvent(nullptr, false, false, "GameThreadEventHandle");
}

void SSGameThread::Tick(float DeltaSeconds)
{
	// do game thread work

	// handle window message
	SSGameWindow::GetPtr()->HandleMessage();

	// 

	// set event
	SetEvent(mGameThreadDoneEventHandle);
}

void SSGameThread::WaitForGameThread(const DWORD WaitTime)
{
	WaitForSingleObject(mGameThreadDoneEventHandle, WaitTime);
}


DWORD SSGameThread::mGameThreadId = 0;