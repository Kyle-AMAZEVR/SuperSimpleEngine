
#include "Core.h"
#include "DXFreeCamera.h"

using namespace DirectX;

DXFreeCamera::DXFreeCamera()
	: SSCameraBase()
{
	mEyePosition = XMFLOAT3(-5, 0, 0);
	mUp = XMFLOAT3(0, 1, 0);
	mLookAtPosition = XMFLOAT3(1, 0, 0);
}

void DXFreeCamera::UpdateViewMatrix()
{
	mView = XMMatrixLookAtLH(
		XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&mUp),
		XMLoadFloat3(&mLookAtPosition)
	);
}

void DXFreeCamera::UpdateProjMatrix()
{
	mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNear, mFar);
}

void DXFreeCamera::Update()
{
	UpdateViewMatrix();
	UpdateProjMatrix();
}
