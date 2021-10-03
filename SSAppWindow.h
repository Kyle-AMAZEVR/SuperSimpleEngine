#pragma once


#include "Renderer/SSRendererModulePCH.h"
#include "GameModule/SSGameWindow.h"

class SSAppWindow : public SSGameWindow
{
public:
	SSAppWindow(HINSTANCE hInstance, int nCmdShow);
	void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnEnterSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;
	void OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) override;

private:
	bool mPauseRendering = false;

};