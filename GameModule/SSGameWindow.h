#pragma once

#include "SSGameModule.h"
#include <windows.h>
#include <functional>
#include <map>

class GAMEMODULE_API SSGameWindow
{
public:
	SSGameWindow(HINSTANCE hInstance, int nCmdShow, int nWidth, int nHeight);

	void HandleMessage();

	// get static instance
	static SSGameWindow* GetPtr() { return mInstance; }

	virtual void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnSizing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnEnterSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HINSTANCE GetInstanceHandle() { return InstanceHandle; }
	HWND GetWindowHandle() { return WindowHandle; }
	
	int Run();

	int GetWindowWidth() const { return mWindowWidth; }
	int GetWindowHeight() const { return mWindowHeight; }

protected:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	WORD RegisterWindowClass(HINSTANCE hInstance);

protected:
	using MsgHandlerType = std::function<void(SSGameWindow*, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;
	
	static std::map<unsigned int, MsgHandlerType> MsgHandlerMap;

	static SSGameWindow* mInstance;

	HACCEL mAccelTable;

	std::wstring Title;
	std::wstring ClassName;

	WCHAR szTitle[256];
	WCHAR szWindowClass[256];

	HINSTANCE InstanceHandle;
	HWND WindowHandle;

	int mWindowWidth = 1920;
	int mWindowHeight = 1080;
	
};