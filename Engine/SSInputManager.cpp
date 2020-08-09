
#include "Core.h"
#include "SSInputManager.h"
#include "SSCameraManager.h"
#include "SSEngine.h"

void SSInputManager::OnKeyDown(ULONGLONG key)
{
	switch (key)
	{
	case VK_UP:	
		SSCameraManager::Get().MoveFoward(2.f);
		break;
	case VK_DOWN:
		SSCameraManager::Get().MoveBackward(2.f);
		break;
	case VK_LEFT:
		SSCameraManager::Get().RotateYaw(-0.01f);
		break;
	case VK_RIGHT:
		SSCameraManager::Get().RotateYaw(0.01f);
		break;

	case VK_F1:
		SSDX11Engine::Get().ToggleGBufferDumpMode();
		break;
	
	}
}

void SSInputManager::OnKeyUp(ULONGLONG keyCode)
{
	
}

void SSInputManager::OnMouseLBtnDown(int x, int y)
{
	mMouseLBtnDownPosX = x;
	mMouseLBtnDownPosY = y;

	mLastMousePosX = x;
	mLastMousePosY = y;

	mMouseLBtnDown = true;
}

void SSInputManager::OnMouseLBtnUp()
{
	mMouseLBtnDown = false;
}


void SSInputManager::OnMouseRBtnDown(int x, int y)
{

}

void SSInputManager::OnMouseMove(int x, int y)
{
	if(mMouseLBtnDown)
	{
		SSCameraManager::Get().RotateYaw((x - mLastMousePosX) * 0.01f);
		SSCameraManager::Get().RotatePitch((y - mLastMousePosY) * -0.01f);

		mLastMousePosX = x;
		mLastMousePosY = y;
	}
}