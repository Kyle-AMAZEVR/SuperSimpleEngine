#pragma once
#include "DirectXMath.h"

class ENGINE_API DXMathHelper
{

public :
    static DirectX::XMMATRIX IdentityMatrix4X4;
    static DirectX::XMFLOAT4 UnitX4;
    static DirectX::XMFLOAT4 UnitY4;
    static DirectX::XMFLOAT4 UnitZ4;
    static DirectX::XMFLOAT4 UnitW4;

    static DirectX::XMFLOAT3 UnitX3;
    static DirectX::XMFLOAT3 UnitY3;
    static DirectX::XMFLOAT3 UnitZ3;
    
};