
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


