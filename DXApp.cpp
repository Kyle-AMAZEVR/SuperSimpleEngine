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
#include "DXApp.h"
#include "Renderer/Core.h"
#include "Engine/SSDX11Engine.h"
#include "GameModule/SSGameWindow.h"
#include "SSAppWindow.h"
#include "SSEngine12.h"
#include "SSGameThread.h"
#include "SSEngineBase.h"
#include "SSSharedRenderData.h"



#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name


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

    AllocConsole();
    FILE* fp{};

    freopen_s(&fp, "CONIN&", "r", stdin);

    freopen_s(&fp, "CONOUT&", "w", stdout);

    freopen_s(&fp, "CONOUT&", "w", stderr);


	SSEngineBase::MainThreadId = GetCurrentThreadId();

	SSGameWindow* GameWindow = new SSAppWindow(hInstance, nCmdShow);

	check(SSGameWindow::GetPtr() != nullptr);

	GEngine = SSDX11Engine::GetPtr();
	
	GEngine->Run();

	return 0;
}




//
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
