#pragma once

#include "SSGameThread.h"
#include "SSRenderingThread.h"

using namespace std;

class SSGameThread;
class SSRenderingThread;


class COMMON_API SSEngineBase
{
public:
	SSEngineBase() = default;
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	virtual void DrawScene() = 0;

	

	static SSGameThread* GetGameThread()  { return mGameThread; }
	static SSRenderingThread* GetRenderingThread()  { return mRenderingThread; }

protected:
	virtual bool CreateDevice() = 0;

	static SSGameThread* mGameThread;
	static SSRenderingThread* mRenderingThread;

	int mBufferWidth = 1024;
	int mBufferHeight = 768;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
};