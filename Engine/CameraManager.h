
#pragma once

#include "Camera.h"
#include "Singleton.h"

class ENGINE_API SSCameraManager : public Singleton<SSCameraManager>
{
public:
	SSCameraManager();
    ~SSCameraManager();

	XMMATRIX GetCurrentCameraView() const;
	XMMATRIX GetCurrentCameraProj() const;
	XMMATRIX GetCurrentCameraViewProj() const;

	void IncreaseCurrentCameraFOV(float amount);
	void DecreaseCurrentCameraFOV(float amount);
	void SetCurrentCameraFOV(float fov);
	void SetCurrentCameraAspectRatio(float ratio);
	
	void UpdateCurrentCamera();

    class SSCameraBase* GetCurrentCamera() { return mCurrentCamera; }

protected:
    class SSCameraBase* mCurrentCamera = nullptr;
};