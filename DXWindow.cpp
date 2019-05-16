
#include "DXWindow.h"
#include "resource.h"



OpenGLWindow::OpenGLWindow(HINSTANCE _hInstance, int nCmdShow)	
	: InstanceHandle(_hInstance)
{
	MsgHandlerMap[WM_PAINT]		= &OpenGLWindow::OnPaint;
	MsgHandlerMap[WM_SIZE]		= &OpenGLWindow::OnSize;
	MsgHandlerMap[WM_CREATE]	= &OpenGLWindow::OnCreate;	
	MsgHandlerMap[WM_DESTROY]	= &OpenGLWindow::OnDestroy;
	MsgHandlerMap[WM_KEYDOWN] = &OpenGLWindow::OnKeyDown;
	MsgHandlerMap[WM_LBUTTONDOWN] = &OpenGLWindow::OnMouseLDown;
	MsgHandlerMap[WM_LBUTTONUP] = &OpenGLWindow::OnMouseLUp;
	MsgHandlerMap[WM_MOUSEMOVE] = &OpenGLWindow::OnMouseMove;
	MsgHandlerMap[WM_ENTERSIZEMOVE] = &OpenGLWindow::OnEnterSizeMoveStart;
	MsgHandlerMap[WM_EXITSIZEMOVE] = &OpenGLWindow::OnEnterSizeMoveEnd;

	LoadStringW(_hInstance, IDS_APP_TITLE, szTitle, 256);
	LoadStringW(_hInstance, IDC_MODERNOPENGL, szWindowClass, 256);

	Title = szTitle;
	ClassName = szWindowClass;

	auto Result = RegisterWindowClass(_hInstance);

	InitInstance(_hInstance, nCmdShow);	
}

void OpenGLWindow::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	DefWindowProc(hWnd, message, wParam, lParam);	
}

void OpenGLWindow::OnMouseMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void OpenGLWindow::OnEnterSizeMoveStart(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void OpenGLWindow::OnEnterSizeMoveEnd(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

}

void OpenGLWindow::OnMouseLDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void OpenGLWindow::OnMouseLUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

void OpenGLWindow::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	
	
	
	EndPaint(hWnd, &ps);
}

void OpenGLWindow::OnCreate(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{

}

void OpenGLWindow::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	

	PostQuitMessage(0);	
}

void OpenGLWindow::OnKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	
}

LRESULT CALLBACK OpenGLWindow::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto iter = MsgHandlerMap[message];

	if (iter == nullptr)
	{
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	else
	{		
		//iter((*MyEngine->MainWindow.get()), hWnd, message, wParam, lParam);
	}

	return 0;
}

int OpenGLWindow::Run()
{
	HACCEL hAccelTable = LoadAccelerators(InstanceHandle, MAKEINTRESOURCE(IDC_MODERNOPENGL));

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

BOOL OpenGLWindow::InitInstance(HINSTANCE hInstance, int nCmdShow)
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

WORD OpenGLWindow::RegisterWindowClass(HINSTANCE hInstance)
{	
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = &OpenGLWindow::WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
 	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MODERNOPENGL));	
 	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
 	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
 	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_MODERNOPENGL);
	wcex.lpszClassName = szWindowClass;
 	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);	
}

std::map<unsigned int, OpenGLWindow::MsgHandlerType> OpenGLWindow::MsgHandlerMap;