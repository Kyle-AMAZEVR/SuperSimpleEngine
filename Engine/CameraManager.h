
#pragma once

#include "Camera.h"
#include "Singleton.h"

class ENGINE_API CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager();
    ~CameraManager();

    class SSCameraBase* GetCurrentCamera() { return mCurrentCamera; }


protected:
    class SSCameraBase* mCurrentCamera = nullptr;
};