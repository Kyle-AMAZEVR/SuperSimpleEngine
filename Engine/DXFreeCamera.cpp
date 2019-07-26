
#include "Core.h"
#include "DXFreeCamera.h"

using namespace DirectX;

SSFreeCamera::SSFreeCamera()
	: SSCameraBase()
{
	mEyePosition = XMFLOAT3(15, 15, -10);
	mUp = XMFLOAT3(0, 1, 0);
	mLookAtPosition = XMFLOAT3(0, 0, 0);
	
}

void SSFreeCamera::UpdateViewMatrix()
{
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
}
