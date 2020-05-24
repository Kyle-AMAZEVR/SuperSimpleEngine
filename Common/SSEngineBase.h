#pragma once

#include "SSGameThread.h"
#include "SSRenderingThread.h"

using namespace std;

class COMMON_API SSEngineBase
{
public:
	SSEngineBase() = default;
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	virtual void DrawScene() = 0;

protected:
	virtual bool CreateDevice() = 0;

	unique_ptr<class SSGameThread> mGameThread;
	unique_ptr<class SSRenderingThread> mRenderingThread;	

	int mBufferWidth = 1024;
	int mBufferHeight = 768;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
};