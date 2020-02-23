
#pragma once

#include "SSCameraBase.h"
#include "Singleton.h"

class ENGINE_API SSCameraManager : public Singleton<SSCameraManager>
{
public:
	SSCameraManager();
    ~SSCameraManager();

	XMMATRIX GetCurrentCameraView() const;
	XMMATRIX GetCurrentCameraProj() const;
	XMMATRIX GetCurrentCameraViewProj() const;
	XMMATRIX GetCurrentCameraMVP() const;
	XMMATRIX GetCurrentCameraTranslation() const;

	void IncreaseCurrentCameraFOV(float amount);
	void DecreaseCurrentCameraFOV(float amount);
	void SetCurrentCameraFOV(float fov);
	void SetCurrentCameraAspectRatio(float ratio);
	
	void UpdateCurrentCamera();

	void MoveFoward(float amount);
	void MoveBackward(float amount);
	void RotateYaw(float amount);
	void RotatePitch(float amount);

    class SSCameraBase* GetCurrentCamera() { return mCurrentCamera; }

protected:
    class SSCameraBase* mCurrentCamera = nullptr;
};