#pragma once
#include "DirectXMath.h"

class ENGINE_API DXMathHelper
{

public :
    static DirectX::XMMATRIX IdentityMatrix;
    static DirectX::XMFLOAT4 UnitX;
    static DirectX::XMFLOAT4 UnitY;
    static DirectX::XMFLOAT4 UnitZ;
    static DirectX::XMFLOAT4 UnitW;
};