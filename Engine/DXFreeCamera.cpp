
#include "Core.h"
#include "DXFreeCamera.h"

using namespace DirectX;

SSFreeCamera::SSFreeCamera()
	: SSCameraBase()
{
	mEyePosition = XMFLOAT3(5, 5, -10);
	mUp = XMFLOAT3(0, 1, 0);
	mLookAtPosition = XMFLOAT3(0, 0, 0);
	
}

void SSFreeCamera::UpdateViewMatrix()
{
	XMFLOAT4X4 matrix;
	XMStoreFloat4x4(&matrix, mRotation);	

	DirectX::XMFLOAT3 lookDir;
	lookDir.x = matrix._11;
	lookDir.y = matrix._12;
	lookDir.z = matrix._13;	

	mView = XMMatrixLookToLH(XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&lookDir),
		XMLoadFloat3(&mUp));

	/*mView = XMMatrixLookAtLH(XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&mLookAtPosition),
		XMLoadFloat3(&mUp));*/
}

void SSFreeCamera::UpdateProjMatrix()
{
	mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNear, mFar);
}

void SSFreeCamera::Update()
{
	UpdateViewMatrix();
	UpdateProjMatrix();
	UpdateRotationMatrix();
}

XMMATRIX SSFreeCamera::GetTranslation() const
{
	return XMMatrixTranslation(mEyePosition.x, mEyePosition.y, mEyePosition.z);
}

void SSFreeCamera::RotatePitch(float amount)
{
	mPitch += amount;
}
void SSFreeCamera::RotateYaw(float amount)
{
	mYaw += amount;	
}

void SSFreeCamera::UpdateRotationMatrix()
{
	mRotation = XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0);
}