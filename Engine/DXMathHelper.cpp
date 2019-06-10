

#include "Core.h"
#include "DXMathHelper.h"
#include "DirectXMath.h"

using namespace DirectX;

XMFLOAT4 DXMathHelper::UnitX4 { 1,0,0,0 };
XMFLOAT4 DXMathHelper::UnitY4 { 0,1,0,0 };
XMFLOAT4 DXMathHelper::UnitZ4 { 0,0,1,0 };
XMFLOAT4 DXMathHelper::UnitW4 { 0,0,0,1 };

DirectX::XMMATRIX DXMathHelper::IdentityMatrix4X4
{
    XMLoadFloat4(&UnitX4),
    XMLoadFloat4(&UnitY4),
    XMLoadFloat4(&UnitZ4),
    XMLoadFloat4(&UnitW4)
};