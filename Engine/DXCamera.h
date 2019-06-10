#pragma once

#include "Camera.h"
#include "DirectXMath.h"

class ENGINE_API DXFreeCamera : public CameraBase
{
public:


protected:
	virtual void UpdateViewMatrix() override;
	DirectX::XMFLOAT3 mEyeLocation;
	DirectX::XMFLOAT3 mLookAt;
};