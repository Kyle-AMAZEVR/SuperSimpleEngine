#pragma once



#include <vector>

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;



class COMMON_API SSRenderer
{
public:
	SSRenderer();
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	virtual void DrawScene() = 0;

protected:
	virtual bool CreateDevice() = 0;
	virtual bool CreateSwapChain() = 0;

	int mBufferWidth = 1024;
	int mBufferHeight = 768;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
};
