// ModernOpenGL.cpp : Defines the entry point for the application.
//


#include "DXWindow.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine); 	

 	auto MainWindow =std::make_unique<OpenGLWindow>(hInstance, nCmdShow);

	int nResult = MainWindow->Run();

	return nResult;
}