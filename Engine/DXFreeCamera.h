#pragma once


#include "Camera.h"
#include "DirectXMath.h"

class ENGINE_API DXFreeCamera : public CameraBase
{
public:
	DXFreeCamera();

protected:
	virtual void UpdateViewMatrix() override;
	virtual void UpdateProjMatrix() override;

	DirectX::XMFLOAT3 mEyeLocation;
	DirectX::XMFLOAT3 mLookAtPosition;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;	
};