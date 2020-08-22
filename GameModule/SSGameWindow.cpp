#include "SSCommon.h"
#include "SSGameWindow.h"
#include "SSTimer.h"
#include "SSInputManager.h"

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
	check(mInstance == nullptr);

	MsgHandlerMap[WM_PAINT] = &SSGameWindow::OnPaint;
	MsgHandlerMap[WM_SIZE] = &SSGameWindow::OnSize;
	MsgHandlerMap[WM_CREATE] = &SSGameWindow::OnCreate;
	MsgHandlerMap[WM_DESTROY] = &SSGameWindow::OnDestroy;
	MsgHandlerMap[WM_KEYDOWN] = &SSGameWindow::OnKeyDown;
	MsgHandlerMap[WM_LBUTTONDOWN] = &SSGameWindow::OnMouseLDown;
	MsgHandlerMap[WM_LBUTTONUP] = &SSGameWindow::OnMouseLUp;
	MsgHandlerMap[WM_MOUSEMOVE] = &SSGameWindow::OnMouseMove;
	MsgHandlerMap[WM_ENTERSIZEMOVE] = &SSGameWindow::OnEnterSizeMove;
	MsgHandlerMap[WM_EXITSIZEMOVE] = &SSGameWindow::OnExitSizeMove;
	
	LoadStringW(_hInstance, IDS_APP_TITLE, szTitle, 256);
	LoadStringW(_hInstance, IDC_DXENGINE, szWindowClass, 256);

	Title = szTitle;
	ClassName = szWindowClass;

	mInstance = this;

	auto Result = RegisterWindowClass(_hInstance);

	InitInstance(_hInstance, nCmdShow);
}

void SSGameWindow::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);


	//renderingThread.WaitForRenderingThread();

	// TODO: Add any drawing code that uses hdc here...            
	EndPaint(hWnd, &ps);
}

void SSGameWindow::OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
}

void SSGameWindow::OnEnterSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("OnEnterSizeMove\n");
}

void SSGameWindow::OnSizing(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("OnEnterSizing\n");
}


void SSGameWindow::OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	OutputDebugStringA("OnExitSizeMove\n");
}

void SSGameWindow::OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
}

void SSGameWindow::OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void SSGameWindow::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	PAINTSTRUCT ps;
	HDC hdc;
	hdc = BeginPaint(hWnd, &ps);


	//renderingThread.WaitForRenderingThread();

	// TODO: Add any drawing code that uses hdc here...            
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
	SSInputManager::Get().OnKeyDown(wParam);
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
		iter(mInstance, hWnd, message, wParam, lParam);
	}

	return 0;
}

int SSGameWindow::Run()
{
	mAccelTable = LoadAccelerators(InstanceHandle, MAKEINTRESOURCE(IDC_DXENGINE));

	MSG msg{};

	SSGameTimer Timer;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, mAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		// gameThread Tick
		
		Timer.Tick();
	}



	return (int)msg.wParam;
}

void SSGameWindow::HandleMessage()
{
	MSG msg{};

	if(GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, mAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}		
	}
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

SSGameWindow* SSGameWindow::mInstance = nullptr;
std::map<unsigned int, SSGameWindow::MsgHandlerType> SSGameWindow::MsgHandlerMap;