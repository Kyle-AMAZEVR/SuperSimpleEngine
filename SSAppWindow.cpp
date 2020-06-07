
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

	if(GEngine != nullptr)
	{
		if (GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->ExecuteInRenderingThread([width, height]()
			{
				GEngine->OnWindowResize(width, height);
			});
		}
	}
	
}
