
#include "Core.h"
#include "DXFreeCamera.h"

using namespace DirectX;

SSFreeCamera::SSFreeCamera()
	: SSCameraBase()
{
	mEyePosition = XMFLOAT3(0, 0, -50);
	mUp = XMFLOAT3(0, 1, 0);
	mLookAtPosition = XMFLOAT3(0, 0, 0);
	
}

void SSFreeCamera::UpdateViewMatrix()
{
	/*XMFLOAT3 tempLookAtDir(0, 0, 1);
	mView = XMMatrixLookToLH(
		XMLoadFloat3(&mEyePosition),
		XMLoadFloat3(&tempLookAtDir),
		XMLoadFloat3(&mUp)
	);*/

	mView = XMMatrixLookAtLH(XMLoadFloat3(&mEyeLocation), 
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
}
