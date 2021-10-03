
#pragma once

#include "SSGameModule.h"
#include "SSCameraBase.h"

using namespace DirectX;

class GAMEMODULE_API SSCameraManager
{
public:
	SSCameraManager();
    ~SSCameraManager();

	static SSCameraManager& Get();
	static SSCameraManager* GetPtr();

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
	static SSCameraManager* mInstance;
};