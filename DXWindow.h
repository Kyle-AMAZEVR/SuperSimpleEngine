#pragma once


#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>
#include <functional>
#include <map>

class OpenGLWindow
{
public:
	OpenGLWindow(){}
	OpenGLWindow(HINSTANCE hInstance, int nCmdShow);

	void OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	void OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnEnterSizeMoveStart(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	void OnEnterSizeMoveEnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	
	HINSTANCE GetInstanceHandle() { return InstanceHandle; }
	HWND GetWindowHandle() { return WindowHandle; }

	int Run();
protected:

	BOOL InitInstance(HINSTANCE hInstance, int nCmdShow);
	WORD RegisterWindowClass(HINSTANCE hInstance);
	

protected:
	using MsgHandlerType = std::function<void(OpenGLWindow&, HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)>;

	static std::map<unsigned int, MsgHandlerType> MsgHandlerMap;	

	std::wstring Title;
	std::wstring ClassName;

	WCHAR szTitle[256];
	WCHAR szWindowClass[256];
	
	HINSTANCE InstanceHandle;
	HWND WindowHandle;	
};