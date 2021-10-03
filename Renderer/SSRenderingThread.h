#pragma once
#include "SSRendererModulePCH.h"
#include <functional>
#include <deque>


class DX11RENDERER_API SSRenderingThread
{
public:	
	SSRenderingThread(class SSRenderer* pRenderer);
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

	bool IsRunning() { return bIsRunning; }
    void ExecuteInRenderingThread(std::function<void()>&& lambdaFunction);

    static bool IsInRenderingThread();

	HANDLE GetRenderingDoneEventHandle() { return mRenderingDoneEventHandle; }

	void WaitForRenderingThread(const DWORD WaitTime = INFINITE);

	void SetGameThreadDone();

	void SetRenderer(class SSRenderer* renderer);

	void PauseRendering();
	void ResumeRendering();


protected:

	HANDLE mGameThreadWaitHandle = nullptr;
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

	class SSRenderer* mRenderer = nullptr;
};