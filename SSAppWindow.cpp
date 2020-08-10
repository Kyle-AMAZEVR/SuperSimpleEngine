
#include "SSAppWindow.h"
#include "SSEngineBase.h"

SSAppWindow::SSAppWindow(HINSTANCE hInstance, int nCmdShow)
	: SSGameWindow(hInstance, nCmdShow)
{
	
}

void SSAppWindow::OnDestroy(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GEngine)
	{
		GEngine->RequestExit();
	}
}

void SSAppWindow::OnSize(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	auto width = static_cast<int>(LOWORD(lParam));
	auto height = static_cast<int>(HIWORD(lParam));	
	
	windowWidth = width;
	windowHeight = height;

	if (GEngine != nullptr)
	{
		if (GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->ExecuteInRenderingThread([this]()
			{				
				GEngine->OnWindowResize(windowWidth, windowHeight);				
			});
		}
	}
}

void SSAppWindow::OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (GEngine != nullptr)
	{
		if (GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->ExecuteInRenderingThread([this]()
			{
				GEngine->GetRenderingThread()->ResumeRendering();
				GEngine->OnWindowResize(windowWidth, windowHeight);
				mPauseRendering = false;
			});
		}
	}
}

void SSAppWindow::OnEnterSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(GEngine!=nullptr)
	{
		if(GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->PauseRendering();
			mPauseRendering = true;
		}
	}
}

void SSAppWindow::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	if(mPauseRendering == false)
	{
		SSGameWindow::OnPaint(hWnd, message, wParam, lParam);
	}
}