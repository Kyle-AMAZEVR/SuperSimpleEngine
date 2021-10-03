
#include "SSGameModule.h"
#include "SSFreeCamera.h"

using namespace DirectX;

SSFreeCamera::SSFreeCamera()
	: SSCameraBase()
{
	mEyePosition = XMFLOAT3(0, 0, 0);
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

	mLookAtPosition = mEyePosition;
	mLookAtPosition.x += lookDir.x;
	mLookAtPosition.y += lookDir.y;
	mLookAtPosition.z += lookDir.z;

	mView = XMMatrixLookAtLH(XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&mLookAtPosition),
		XMLoadFloat3(&mUp));
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
	mRotation = XMMatrixRotationZ(mPitch) * XMMatrixRotationY(mYaw);
}

void SSFreeCamera::MoveFoward(float amount)
{
	XMFLOAT4X4 rotmat;
	XMStoreFloat4x4(&rotmat, mRotation);

	auto vMoveDir = XMFLOAT3(rotmat._11, rotmat._12, rotmat._13);
	
	mEyePosition.x += (vMoveDir.x *amount);
	mEyePosition.y += (vMoveDir.y * amount);
	mEyePosition.z += (vMoveDir.z *amount);	
}

void SSFreeCamera::MoveBackward(float amount)
{
	XMFLOAT4X4 rotmat;
	XMStoreFloat4x4(&rotmat, mRotation);

	auto vMoveDir = XMFLOAT3(rotmat._11, rotmat._12, rotmat._13);

	mEyePosition.x -= (vMoveDir.x *amount);
	mEyePosition.y -= (vMoveDir.y * amount);
	mEyePosition.z -= (vMoveDir.z *amount);
}
