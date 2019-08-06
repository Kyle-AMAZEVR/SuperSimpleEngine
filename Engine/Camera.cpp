
#include "Core.h"
#include "Camera.h"
#include "DXMathHelper.h"

XMMATRIX SSCameraBase::GetViewProj() const 
{
	return mView * mProj;
}

XMMATRIX SSCameraBase::GetTranslation() const
{
	return SSMathHelper::IdentityMatrix4X4;
}

XMMATRIX SSCameraBase::GetRotation() const
{
	return XMMatrixRotationRollPitchYaw(mPitch, mYaw, 0);
}


