
#include "Core.h"
#include "FreeCamera.h"

void FreeCamera::UpdateViewMatrix()
{
    mView = XMMatrixLookAtLH(mEyePosition, mLookAtPosition, mUp);

}

void FreeCamera::UpdateProjMatrix()
{
    mProj = XMMatrixPerspectiveFovLH(mFOV, mAspectRatio, mNear, mFar);
}