#pragma once
#include <windows.h>

#include <functional>
#include <map>

class SSGameWindow
{
public:
	SSGameWindow(HINSTANCE hInstance, int nCmdShow);

	// get static instance
	static SSGameWindow* GetPtr() { return mInstance; }

	virtual void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnEnterSizeMoveStart(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	virtual void OnEnterSizeMoveEnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	HINSTANCE GetInstanceHandle() { return InstanceHandle; }
	HWND GetWindowHandle() { return WindowHandle; }
	
	int Run();

protected:
	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	WORD RegisterWindowClass(HINSTANCE hInstance);

protected:
	using MsgHandlerType = std::function<void(SSGameWindow*, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;
	
	static std::map<unsigned int, MsgHandlerType> MsgHandlerMap;

	static SSGameWindow* mInstance;

	std::wstring Title;
	std::wstring ClassName;

	WCHAR szTitle[256];
	WCHAR szWindowClass[256];

	HINSTANCE InstanceHandle;
	HWND WindowHandle;
	
};