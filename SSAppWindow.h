#pragma once


#include "DXApp.h"
#include "Engine/Core.h"
#include "Engine/SSEngine.h"
#include "Common/SSRenderingThread.h"
#include "Engine/SSInputManager.h"
#include "Common/SSGameWindow.h"

class SSAppWindow : public SSGameWindow
{
public:
	SSAppWindow(HINSTANCE hInstance, int nCmdShow);
	void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;



};