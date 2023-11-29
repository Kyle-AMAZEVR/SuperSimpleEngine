#pragma once

#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSRenderer
{
public:
	SSRenderer();
	virtual void Initialize(HWND windowHandle) = 0;
	virtual void Shutdown() = 0;
	virtual void OnWindowResize(int newWidth, int newHeight) = 0;
	virtual void DrawScene() = 0;
	virtual void PauseRendering() { mPaused = true; }
	virtual void ResumeRendering() { mPaused = false; }
	virtual void UpdateRenderingObjects() = 0;

protected:	

	
	bool mPaused = false;
};
