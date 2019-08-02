#pragma once


#include "Camera.h"
#include "DirectXMath.h"

class ENGINE_API SSFreeCamera : public SSCameraBase
{
public:
	SSFreeCamera();
	virtual void Update() override;
	virtual XMMATRIX GetTranslation() const override;
	virtual void RotatePitch(float amount) override;
	virtual void RotateYaw(float amount) override;
protected:
	virtual void UpdateViewMatrix() override;
	virtual void UpdateProjMatrix() override;
	virtual void UpdateRotationMatrix() override;
	
	
	DirectX::XMFLOAT3 mLookAtPosition;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;	
};