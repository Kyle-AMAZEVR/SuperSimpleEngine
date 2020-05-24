#include "SSCommon.h"
#include "SSGameWindow.h"

//{{NO_DEPENDENCIES}}
// Microsoft Visual C++ generated include file.
// Used by DXEngine.rc

#define IDS_APP_TITLE			103

#define IDR_MAINFRAME			128
#define IDD_DXENGINE_DIALOG	102
#define IDD_ABOUTBOX			103
#define IDM_ABOUT				104
#define IDM_EXIT				105
#define IDI_DXENGINE			107
#define IDI_SMALL				108
#define IDC_DXENGINE			109
#define IDC_MYICON				2
#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif
// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

#define MAX_LOADSTRING 100
// Global Variables:
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


// Next default values for new objects
//
#ifdef APSTUDIO_INVOKED
#ifndef APSTUDIO_READONLY_SYMBOLS

#define _APS_NO_MFC					130
#define _APS_NEXT_RESOURCE_VALUE	129
#define _APS_NEXT_COMMAND_VALUE		32771
#define _APS_NEXT_CONTROL_VALUE		1000
#define _APS_NEXT_SYMED_VALUE		110
#endif
#endif

SSGameWindow::SSGameWindow(HINSTANCE _hInstance, int nCmdShow)
	: InstanceHandle(_hInstance)
{
	MsgHandlerMap[WM_PAINT] = &SSGameWindow::OnPaint;
	MsgHandlerMap[WM_SIZE] = &SSGameWindow::OnSize;
	MsgHandlerMap[WM_CREATE] = &SSGameWindow::OnCreate;
	MsgHandlerMap[WM_DESTROY] = &SSGameWindow::OnDestroy;
	MsgHandlerMap[WM_KEYDOWN] = &SSGameWindow::OnKeyDown;
	MsgHandlerMap[WM_LBUTTONDOWN] = &SSGameWindow::OnMouseLDown;
	MsgHandlerMap[WM_LBUTTONUP] = &SSGameWindow::OnMouseLUp;
	MsgHandlerMap[WM_MOUSEMOVE] = &SSGameWindow::OnMouseMove;
	MsgHandlerMap[WM_ENTERSIZEMOVE] = &SSGameWindow::OnEnterSizeMoveStart;
	MsgHandlerMap[WM_EXITSIZEMOVE] = &SSGameWindow::OnEnterSizeMoveEnd;

	LoadStringW(_hInstance, IDS_APP_TITLE, szTitle, 256);
	LoadStringW(_hInstance, IDC_DXENGINE, szWindowClass, 256);

	Title = szTitle;
	ClassName = szWindowClass;

	auto Result = RegisterWindowClass(_hInstance);

	InitInstance(_hInstance, nCmdShow);
}

void SSGameWindow::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	DefWindowProc(hWnd, message, wParam, lParam);
}

void SSGameWindow::OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void SSGameWindow::OnEnterSizeMoveStart(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void SSGameWindow::OnEnterSizeMoveEnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void SSGameWindow::OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto X = static_cast<float>(LOWORD(lParam));
	auto Y = static_cast<float>(HIWORD(lParam));

	
}

void SSGameWindow::OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void SSGameWindow::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);

	

	EndPaint(hWnd, &ps);
}

void SSGameWindow::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void SSGameWindow::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	PostQuitMessage(0);
}

void SSGameWindow::OnKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

LRESULT CALLBACK SSGameWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto iter = MsgHandlerMap[message];

	if (iter == nullptr)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{
		//iter(*this, hWnd, message, wParam, lParam);
	}

	return 0;
}

int SSGameWindow::Run()
{
	HACCEL hAccelTable = LoadAccelerators(InstanceHandle, MAKEINTRESOURCE(IDC_DXENGINE));

	MSG msg{};

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}

BOOL SSGameWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	WindowHandle = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

	if (!WindowHandle)
	{
		return FALSE;
	}

	ShowWindow(WindowHandle, nCmdShow);
	UpdateWindow(WindowHandle);

	return TRUE;
}

WORD SSGameWindow::RegisterWindowClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &SSGameWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DXENGINE));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DXENGINE);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

std::map<unsigned int, SSGameWindow::MsgHandlerType> SSGameWindow::MsgHandlerMap;