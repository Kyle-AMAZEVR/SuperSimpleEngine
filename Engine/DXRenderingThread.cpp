
#include "Core.h"
#include "DXRenderingThread.h"
#include "DXEngine.h"
#include "Windows.h"

void DXRenderingThread::Start(HWND handle)
{
    mWindowHandle = handle;
	mThreadHandle = CreateThread(nullptr, 0, &DXRenderingThread::Run, this, 0, &mRenderingThreadId);
	InitializeCriticalSection(&mCriticalSection);
}

DWORD DXRenderingThread::Run()
{
	// init engine
	DXEngine::Get().Initialize(mWindowHandle);

	while (1)
	{
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

		DXEngine::Get().DrawScene();

		if (bRequestExit)
		{
			return 0;
		}
	}	
}

DWORD DXRenderingThread::Run(LPVOID param)
{
	DXRenderingThread* threadInstance = (DXRenderingThread*)param;
	return threadInstance->Run();
}

void DXRenderingThread::Join()
{
	DWORD waitResult = WaitForSingleObject(mThreadHandle, INFINITE);
	check(waitResult == WAIT_OBJECT_0);
}

bool DXRenderingThread::IsInRenderingThread()
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

void DXRenderingThread::ExecuteInRenderingThread(std::function<void()>&& lambdaFunction)
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

DWORD DXRenderingThread::mRenderingThreadId = 0;