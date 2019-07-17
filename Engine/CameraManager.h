
#pragma once

#include "Camera.h"
#include "Singleton.h"

class ENGINE_API CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager();
    ~CameraManager();

    class CameraBase* GetCurrentCamera() { return mCurrentCamera; }


protected:
    class CameraBase* mCurrentCamera = nullptr;
};