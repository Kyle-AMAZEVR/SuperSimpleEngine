
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

XMMATRIX SSCameraManager::GetCurrentCameraTranslation() const
{
	check(mCurrentCamera != nullptr);
	return mCurrentCamera->GetTranslation();
}

void SSCameraManager::RotateYaw(float amount)
{
	mCurrentCamera->RotateYaw(amount);
}

void SSCameraManager::RotatePitch(float amount)
{
	mCurrentCamera->RotatePitch(amount);
}

void SSCameraManager::MoveFoward(float amount)
{
	mCurrentCamera->MoveFoward(amount);
}

void SSCameraManager::MoveBackward(float amount)
{
	mCurrentCamera->MoveBackward(amount);
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

void SSCameraManager::SetCurrentCameraFOV(float fov)
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->SetFOV(fov);
}

void SSCameraManager::IncreaseCurrentCameraFOV(float amount)
{
	float newFOV = mCurrentCamera->GetFOV() + amount;
	mCurrentCamera->SetFOV(newFOV);
}

void SSCameraManager::DecreaseCurrentCameraFOV(float amount)
{
	float newFOV = mCurrentCamera->GetFOV() - amount;

	if (newFOV <= 0)
	{
		newFOV = 0.001f;
	}

	mCurrentCamera->SetFOV(newFOV);
}



void SSCameraManager::SetCurrentCameraAspectRatio(float ratio)
{
	check(mCurrentCamera != nullptr);
	mCurrentCamera->SetAspectRatio(ratio);
}

