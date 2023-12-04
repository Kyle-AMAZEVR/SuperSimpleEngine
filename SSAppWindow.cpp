
#include "SSAppWindow.h"
#include "SSEngineBase.h"
#include "SSRenderingThread.h"

SSAppWindow::SSAppWindow(HINSTANCE hInstance, int nCmdShow, int nWidth, int nHeight)
	: SSGameWindow(hInstance, nCmdShow, nWidth, nHeight)
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
	
	mWindowWidth = width;
	mWindowHeight = height;	
}

void SSAppWindow::OnExitSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (GEngine != nullptr)
	{		
		if (GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->ExecuteInRenderingThread([this]()
			{
				GEngine->OnWindowResize(mWindowWidth, mWindowHeight);
				GEngine->GetRenderingThread()->ResumeRendering();
				mPauseRendering = false;
			});
		}
	}
}

void SSAppWindow::OnEnterSizeMove(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	if (GEngine)
	{
		if (GEngine->GetRenderingThread()->IsRunning())
		{
			GEngine->GetRenderingThread()->ExecuteInRenderingThread([this]()
			{
				GEngine->GetRenderingThread()->PauseRendering();
			});
		}
	}
}

void SSAppWindow::OnPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{	
	SSGameWindow::OnPaint(hWnd, message, wParam, lParam);	
}