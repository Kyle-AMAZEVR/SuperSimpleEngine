
#pragma once

#include "Camera.h"
#include "Singleton.h"

class ENGINE_API CameraManager : public Singleton<CameraManager>
{
public:
	CameraManager();
    ~CameraManager();

	XMMATRIX GetCurrentCameraView() const;
	XMMATRIX GetCurrentCameraProj() const;
	XMMATRIX GetCurrentCameraViewProj() const;


    class SSCameraBase* GetCurrentCamera() { return mCurrentCamera; }


protected:
    class SSCameraBase* mCurrentCamera = nullptr;
};