
#include "Core.h"
#include "DXFreeCamera.h"
#include "CameraManager.h"

SSCameraManager::SSCameraManager()
{
    mCurrentCamera = new SSFreeCamera();
}

SSCameraManager::~SSCameraManager()
{
    if(mCurrentCamera)
    {
        delete mCurrentCamera;
    }
}

XMMATRIX SSCameraManager::GetCurrentCameraView() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetView();
}

XMMATRIX SSCameraManager::GetCurrentCameraProj() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetProj();
}

XMMATRIX SSCameraManager::GetCurrentCameraViewProj() const 
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetViewProj();
}

void SSCameraManager::UpdateCurrentCamera()
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->Update();
}
