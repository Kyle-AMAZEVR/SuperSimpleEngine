// DXEngine.cpp : Defines the entry point for the application.
//
//#include "stdafx.h"

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>

#include "d3d11.h"
#include "DXWindow.h"
#include "Engine/Core.h"
#include "Engine/SSEngine.h"
#include "Common/SSRenderingThread.h"
#include "Common/SSTimer.h"
#include "Engine/SSInputManager.h"
#include "Engine/SSFontManager.h"
#include "Windowsx.h"
#include "SSEngine12.h"
#include "SSGameThread.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND WindowHandle;
SSRenderingThread renderingThread;
SSGameThread gameThread;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_DXENGINE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization: 
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    // start rendering thread
	SSDX11Engine* DX11Engine = SSDX11Engine::GetPtr();
	SSDX12Engine* DX12Engine = new SSDX12Engine();

	gameThread.Start(GetCurrentThreadId());
    renderingThread.Start(WindowHandle, DX11Engine);

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DXENGINE));

    MSG msg;

	SSGameTimer Timer;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		Timer.Tick();
		gameThread.Tick(Timer.GetDeltaTime());
    }
    renderingThread.RequestExit(); 
    renderingThread.Join();

	if(DX12Engine != nullptr)
	{
		delete DX12Engine;
	}

    return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DXENGINE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_DXENGINE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable	

   HWND hWnd = CreateWindowW(szWindowClass, L"SuperSimple", WS_OVERLAPPEDWINDOW,
      0, 0, 480, 360, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);   
   WindowHandle = hWnd;
   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) 
{
    static int width = 0;
    static int height = 0;
	

    switch (message)
    {
    case WM_COMMAND:
        { 
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
			HDC hdc;
            hdc = BeginPaint(hWnd, &ps);

			
			//renderingThread.WaitForRenderingThread();

            // TODO: Add any drawing code that uses hdc here...            
            EndPaint(hWnd, &ps);
        }
        break;

    case WM_SIZE:
        {
            width = static_cast<int>(LOWORD(lParam));
            height = static_cast<int>(HIWORD(lParam));
			if (renderingThread.IsRunning())
			{
				renderingThread.ExecuteInRenderingThread([]()
				{
					//SSEngine12::Get().OnWindowResize(width, height);
					SSDX11Engine::Get().OnWindowResize(width, height);
				});
			}
        }
        break;

	case WM_KEYDOWN:
		SSInputManager::Get().OnKeyDown(wParam);
		break;

	case WM_MOUSEMOVE:		
		SSInputManager::Get().OnMouseMove(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)); 
		break;

	case WM_LBUTTONDOWN:		
		SSInputManager::Get().OnMouseLBtnDown(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
		break;

	case WM_LBUTTONUP:
		SSInputManager::Get().OnMouseLBtnUp();
		break;
	
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
