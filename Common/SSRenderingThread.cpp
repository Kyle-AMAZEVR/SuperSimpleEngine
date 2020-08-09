
#include "SSCommon.h"
#include "SSRenderingThread.h"
#include "Windows.h"
#include "SSTimer.h"
#include "SSEngineBase.h"
#include "SSGameThread.h"
#include "SSDX11Renderer.h"

void SSRenderingThread::Start(HWND handle, SSEngineBase* pEngine)
{
	mEngineInstance = pEngine;
    mWindowHandle = handle;
	mThreadHandle = CreateThread(nullptr, 0, &SSRenderingThread::Run, this, 0, &mRenderingThreadId);
	InitializeCriticalSection(&mCriticalSection);
	mEventName = "RenderingThreadDoneEvent";
	mRenderingDoneEventHandle = CreateEvent(nullptr, false, false, mEventName);
}

void SSRenderingThread::WaitForRenderingThread(const DWORD WaitTime)
{
	WaitForSingleObject(mRenderingDoneEventHandle, WaitTime);
}


DWORD SSRenderingThread::Run()
{
	// init engine
	mEngineInstance->Initialize(mWindowHandle);	

	bIsRunning = true;

	SSGameTimer renderingThreadTimer;
	renderingThreadTimer.Tick();

	while (1)
	{
		mEngineInstance->GetGameThread()->WaitForGameThread(33);

		// consume command queue
		{
			EnterCriticalSection(&mCriticalSection);
			if (mCommandQueue.empty() == false)
			{
				for (auto iter = mCommandQueue.begin(); iter != mCommandQueue.end(); iter++)
				{
					(*iter)();
				}
				mCommandQueue.clear();
			}
			LeaveCriticalSection(&mCriticalSection);
		}
		//
		renderingThreadTimer.Tick();

		// Tick Rendering Thread

		mEngineInstance->TickRenderThread(renderingThreadTimer.GetDeltaTime());

		SetEvent(mRenderingDoneEventHandle);

		if (bRequestExit)
		{	
			return 0;
		}
	}	
}

DWORD SSRenderingThread::Run(LPVOID param)
{
	SSRenderingThread* threadInstance = (SSRenderingThread*)param;
	return threadInstance->Run();
}

void SSRenderingThread::Join()
{
	DWORD waitResult = WaitForSingleObject(mThreadHandle, INFINITE);
	check(waitResult == WAIT_OBJECT_0);
}

bool SSRenderingThread::IsInRenderingThread()
{
    if(mRenderingThreadId == GetCurrentThreadId())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void SSRenderingThread::ExecuteInRenderingThread(std::function<void()>&& lambdaFunction)
{
    if(IsInRenderingThread())
    {
        lambdaFunction();
    }
    else
    {        
		EnterCriticalSection(&mCriticalSection);
        mCommandQueue.push_back(std::move(lambdaFunction));
		LeaveCriticalSection(&mCriticalSection);
    }    
}

DWORD SSRenderingThread::mRenderingThreadId = 0;