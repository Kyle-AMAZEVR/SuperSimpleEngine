
#include "Core.h"
#include "Camera.h"

XMMATRIX SSCameraBase::GetViewProj() const 
{
	return mView * mProj;
}

