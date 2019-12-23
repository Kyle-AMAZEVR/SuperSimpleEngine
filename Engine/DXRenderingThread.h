#pragma once


#include <functional>
#include <mutex>
#include <deque>


class ENGINE_API DXRenderingThread 
{
public:
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

	bool IsRunning() { return bIsRunning; }
    void ExecuteInRenderingThread(std::function<void()>&& lambdaFunction);

    static bool IsInRenderingThread();

	HANDLE GetRenderingDoneEventHandle() { return mRenderingDoneEventHandle; }

protected:
	HANDLE mThreadHandle = nullptr;
	static DWORD mRenderingThreadId;
    static DWORD Run(LPVOID param);
	DWORD Run();
	bool bIsRunning = false;
    //
	CRITICAL_SECTION mCriticalSection;
    std::deque<std::function<void()>> mCommandQueue;
	//    

	HANDLE mRenderingDoneEventHandle;
    HWND mWindowHandle;
    bool bRequestExit = false;
	LPCSTR mEventName;
};