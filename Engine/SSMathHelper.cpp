

#include "Core.h"
#include "SSMathHelper.h"
#include "DirectXMath.h"

using namespace DirectX;

XMFLOAT4 SSMathHelper::UnitX4 { 1,0,0,0 };
XMFLOAT4 SSMathHelper::UnitY4 { 0,1,0,0 };
XMFLOAT4 SSMathHelper::UnitZ4 { 0,0,1,0 };
XMFLOAT4 SSMathHelper::UnitW4 { 0,0,0,1 };

XMFLOAT4 SSMathHelper::MinusUnitX4{ -1,0,0,0 };
XMFLOAT4 SSMathHelper::MinusUnitY4{ 0,-1,0,0 };
XMFLOAT4 SSMathHelper::MinusUnitZ4{ 0,0,-1,0 };
XMFLOAT4 SSMathHelper::MinusUnitW4{ 0,0,0,-1 };


XMFLOAT3 SSMathHelper::UnitX3{ 1,0,0 };
XMFLOAT3 SSMathHelper::UnitY3{ 0,1,0 };
XMFLOAT3 SSMathHelper::UnitZ3{ 0,0,1 };

XMFLOAT2 SSMathHelper::UnitX2{ 1,0 };
XMFLOAT2 SSMathHelper::UnitY2{ 0,1 };

XMFLOAT4 SSMathHelper::Zero4 {0,0,0,0};
XMFLOAT3 SSMathHelper::Zero3{ 0,0,0 };
XMFLOAT2 SSMathHelper::Zero2{ 0,0 };


DirectX::XMMATRIX SSMathHelper::IdentityMatrix4X4
{
    XMLoadFloat4(&UnitX4),
    XMLoadFloat4(&UnitY4),
    XMLoadFloat4(&UnitZ4),
    XMLoadFloat4(&UnitW4)
};

XMFLOAT3 origin = XMFLOAT3(0, 0, 0);

//auto negXView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::MinusUnitX4), XMLoadFloat4(&SSMathHelper::MinusUnitY4));
//auto negYView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::UnitY4), XMLoadFloat4(&SSMathHelper::MinusUnitZ4));
//auto posXView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::UnitX4), XMLoadFloat4(&SSMathHelper::MinusUnitY4));
//auto posYView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::MinusUnitY4), XMLoadFloat4(&SSMathHelper::UnitZ4));
//auto negZView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::UnitZ4), XMLoadFloat4(&SSMathHelper::MinusUnitY4));
//auto posZView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&SSMathHelper::MinusUnitZ4), XMLoadFloat4(&SSMathHelper::MinusUnitY4));

XMMATRIX SSMathHelper::NegativeXViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::MinusUnitX4), XMLoadFloat4(&SSMathHelper::UnitY4));
XMMATRIX SSMathHelper::PositiveXViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::UnitX4), XMLoadFloat4(&SSMathHelper::UnitY4));

XMMATRIX SSMathHelper::NegativeYViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::MinusUnitY4), XMLoadFloat4(&SSMathHelper::UnitZ4));
XMMATRIX SSMathHelper::PositiveYViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::UnitY4), XMLoadFloat4(&SSMathHelper::MinusUnitZ4));

XMMATRIX SSMathHelper::NegativeZViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::MinusUnitZ4), XMLoadFloat4(&SSMathHelper::UnitY4));
XMMATRIX SSMathHelper::PositiveZViewMatrix = XMMatrixLookToLH(XMLoadFloat3(&Zero3), XMLoadFloat4(&SSMathHelper::UnitZ4), XMLoadFloat4(&SSMathHelper::UnitY4));


UINT SSMathHelper::CalcMipLevel(UINT nSize)
{
	bool bPowerOfTwo = !(nSize == 0) && !(nSize & (nSize - 1));
	check(bPowerOfTwo);

	UINT mipCount = 0;

	do
	{
		mipCount++;
		nSize /= 2;
	} while (nSize > 0);

	return mipCount;
}