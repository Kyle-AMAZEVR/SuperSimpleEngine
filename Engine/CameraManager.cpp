
#include "Core.h"
#include "FreeCamera.h"
#include "CameraManager.h"

CameraManager::CameraManager()
{
    mCurrentCamera = new FreeCamera();
}

CameraManager::~CameraManager()
{
    if(mCurrentCamera)
    {
        delete mCurrentCamera;
    }
}
