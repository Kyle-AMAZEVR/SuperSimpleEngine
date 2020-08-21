#pragma once
#include <functional>
#include <deque>


class GAMEMODULE_API SSRenderingThread
{
public:	
    void Start(HWND handle, class SSEngineBase* pEngine);
    void Join();
    void RequestExit() { bRequestExit = true;}

	bool IsRunning() { return bIsRunning; }
    void ExecuteInRenderingThread(std::function<void()>&& lambdaFunction);

    static bool IsInRenderingThread();

	HANDLE GetRenderingDoneEventHandle() { return mRenderingDoneEventHandle; }

	void WaitForRenderingThread(const DWORD WaitTime = INFINITE);

	void SetRenderer(class SSRenderer* renderer);

	void PauseRendering();
	void ResumeRendering();

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

	class SSEngineBase* mEngineInstance = nullptr;

	class SSRenderer* mRenderer = nullptr;
};