#pragma once

#include "SSGameThread.h"
#include "SSRenderingThread.h"

using namespace std;

class SSGameThread;
class SSRenderingThread;


class GAMEMODULE_API SSEngineBase
{
public:
	SSEngineBase() = default;
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;	
	virtual void EngineStart();
	

	virtual void Run() = 0;

	virtual void TickGameThread(float deltaTime) = 0;
	virtual void TickRenderThread(float deltaTime) = 0;

	static SSGameThread* GetGameThread()  { return mGameThread; }
	static SSRenderingThread* GetRenderingThread()  { return mRenderingThread; }

	static DWORD MainThreadId;

	void RequestExit() { bRequestExit = true; }

protected:
	

	static SSGameThread* mGameThread;
	static SSRenderingThread* mRenderingThread;

	int mBufferWidth = 1024;
	int mBufferHeight = 768;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;

	bool bRequestExit = false;
};

extern "C" GAMEMODULE_API SSEngineBase* GEngine;