#pragma once

#include "SSCameraBase.h"
#include "DirectXMath.h"

class CORE_API SSOrbitCamera : public SSCameraBase
{
public:
	SSOrbitCamera();

protected:	
	float mDistance = 50.f;
	float mAzimuth = 10.f;
	float mAltitude = 10.f;

	DirectX::XMFLOAT3 mLookAtPosition;
	DirectX::XMFLOAT3 mEyePosition;
	DirectX::XMFLOAT3 mUp;
};