

#pragma once

#include "Camera.h"
#include "DirectXMath.h"

using namespace DirectX;

class ENGINE_API FreeCamera : public CameraBase
{
public:

protected:
    virtual void UpdateViewMatrix() override;
    virtual void UpdateProjMatrix() override;

    FXMVECTOR mEyePosition;
    FXMVECTOR mUp;
    FXMVECTOR mLookAtPosition;
};
