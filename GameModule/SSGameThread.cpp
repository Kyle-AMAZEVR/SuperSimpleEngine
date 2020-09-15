
#include "SSCommon.h"
#include "SSGameThread.h"
#include "SSGameObjectManager.h"
#include "Windows.h"
#include "SSGameWindow.h"
#include "SSCameraManager.h"

SSGameThread::SSGameThread(DWORD gameThreadId)
{
	Start(gameThreadId);
}


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
	mGameThreadTimer.Tick();
	// do game thread work

	// handle window message
	SSGameWindow::GetPtr()->HandleMessage();

	float tickTime = mGameThreadTimer.GetDeltaTime();
	
	SSGameObjectManager::GetPtr()->Tick(mGameThreadTimer.GetDeltaTime());

	SSCameraManager::Get().UpdateCurrentCamera();

	// set event
	SetEvent(mGameThreadDoneEventHandle);
}

void SSGameThread::WaitForGameThread(const DWORD WaitTime)
{
	WaitForSingleObject(mGameThreadDoneEventHandle, WaitTime);	
}


DWORD SSGameThread::mGameThreadId = 0;