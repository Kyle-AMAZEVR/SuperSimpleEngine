
#include "SSOrbitCamera.h"

SSOrbitCamera::SSOrbitCamera()
	: SSCameraBase()
{	
	mUp = XMFLOAT3(0, 1, 0);

	float X = mDistance * DirectX::XMScalarCos(XMConvertToRadians(mAzimuth));
	float Z = mDistance * DirectX::XMScalarSin(XMConvertToRadians(mAzimuth));
	float Y = DirectX::XMScalarSin(XMConvertToRadians(mAltitude));

	mEyePosition = XMFLOAT3(X, Y, Z);

	mLookAtPosition = XMFLOAT3(0, 0, 0);
}

void SSOrbitCamera::UpdateViewMatrix()
{
	float X = mDistance * DirectX::XMScalarCos(XMConvertToRadians(mAzimuth));
	float Z = mDistance * DirectX::XMScalarSin(XMConvertToRadians(mAzimuth));
	float Y = DirectX::XMScalarSin(XMConvertToRadians(mAltitude));

	mEyePosition = XMFLOAT3(X, Y, Z);

	mView = XMMatrixLookAtLH(
		XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&mLookAtPosition),
		XMLoadFloat3(&mUp));
}


void SSOrbitCamera::UpdateProjMatrix()
{
}

void SSOrbitCamera::UpdateRotationMatrix()
{
}
