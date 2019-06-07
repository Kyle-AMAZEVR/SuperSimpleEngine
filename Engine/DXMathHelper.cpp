

#include "Core.h"
#include "DXMathHelper.h"
#include "DirectXMath.h"

using namespace DirectX;

XMFLOAT4 DXMathHelper::UnitX { 1,0,0,0 };
XMFLOAT4 DXMathHelper::UnitY { 1,0,0,0 };
XMFLOAT4 DXMathHelper::UnitZ { 1,0,0,0 };
XMFLOAT4 DXMathHelper::UnitW { 1,0,0,0 };

DirectX::XMMATRIX DXMathHelper::IdentityMatrix
{
    XMLoadFloat4(&UnitX),
    XMLoadFloat4(&UnitY),
    XMLoadFloat4(&UnitZ),
    XMLoadFloat4(&UnitW)
};