

#pragma once

#include "Camera.h"
#include "DirectXMath.h"

using namespace DirectX;

class ENGINE_API FreeCamera : public CameraBase
{
public:
    FreeCamera();
protected:
    virtual void UpdateViewMatrix() override;
    virtual void UpdateProjMatrix() override;    

    XMFLOAT3 mEyePosition;
    XMFLOAT3 mUp;
    XMFLOAT3 mLookAtPosition;    
};
