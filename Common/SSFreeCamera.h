#pragma once


#include "SSCameraBase.h"
#include "DirectXMath.h"

class COMMON_API SSFreeCamera : public SSCameraBase
{
public:
	SSFreeCamera();
	virtual void Update() override;
	virtual DirectX::XMMATRIX GetTranslation() const override;
	virtual void RotatePitch(float amount) override;
	virtual void RotateYaw(float amount) override;
	virtual void MoveFoward(float amount) override;
	virtual void MoveBackward(float amount) override;
protected:
	virtual void UpdateViewMatrix() override;
	virtual void UpdateProjMatrix() override;
	virtual void UpdateRotationMatrix() override;
	
	
	DirectX::XMFLOAT3 mLookAtPosition;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;	
};