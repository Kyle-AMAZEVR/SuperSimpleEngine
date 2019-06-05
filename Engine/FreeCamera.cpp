
#include "Core.h"
#include "FreeCamera.h"

FreeCamera::FreeCamera() 
 : CameraBase()
{
    mEyePosition = XMFLOAT3(0,0,0);
    mUp = XMFLOAT3(0,1,0);
    mLookAtPosition = XMFLOAT3(1,0,0);
}

void FreeCamera::UpdateViewMatrix()
{
    mView = XMMatrixLookAtLH(
        XMLoadFloat3(&mEyePosition),
        XMLoadFloat3(&mUp),
        XMLoadFloat3(&mLookAtPosition)
      );      
}

void FreeCamera::UpdateProjMatrix()
{
    mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNear, mFar);
}