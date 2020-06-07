
#include "SSAppWindow.h"
#include "SSEngineBase.h"

SSAppWindow::SSAppWindow(HINSTANCE hInstance, int nCmdShow)
	: SSGameWindow(hInstance, nCmdShow)
{
	
}

void SSAppWindow::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto width = static_cast<int>(LOWORD(lParam));
	auto height = static_cast<int>(HIWORD(lParam));	

	if (SSDX11Engine::Get().GetRenderingThread()->IsRunning())
	{
		SSDX11Engine::Get().GetRenderingThread()->ExecuteInRenderingThread([width, height]()
		{
			SSDX11Engine::Get().OnWindowResize(width, height);
		});
	}
}
