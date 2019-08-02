
#include "Core.h"
#include "Camera.h"
#include "DXMathHelper.h"

XMMATRIX SSCameraBase::GetViewProj() const 
{
	return mView * mProj;
}

XMMATRIX SSCameraBase::GetTranslation() const
{
	return DXMathHelper::IdentityMatrix4X4;
}

