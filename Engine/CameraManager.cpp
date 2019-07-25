
#include "Core.h"
#include "DXFreeCamera.h"
#include "CameraManager.h"

CameraManager::CameraManager()
{
    mCurrentCamera = new SSFreeCamera();
}

CameraManager::~CameraManager()
{
    if(mCurrentCamera)
    {
        delete mCurrentCamera;
    }
}

XMMATRIX CameraManager::GetCurrentCameraView() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetView();
}

XMMATRIX CameraManager::GetCurrentCameraProj() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetProj();
}


