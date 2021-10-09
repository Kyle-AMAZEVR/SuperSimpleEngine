#pragma once
#include "DirectXMath.h"

using namespace DirectX;

class CORE_API SSMathHelper
{

public :
    static DirectX::XMMATRIX IdentityMatrix4X4;
	static DirectX::XMMATRIX IdentityMatrix3X3;
	static DirectX::XMMATRIX IdentityMatrix2X2;

    static DirectX::XMFLOAT4 UnitX4;
    static DirectX::XMFLOAT4 UnitY4;
    static DirectX::XMFLOAT4 UnitZ4;
    static DirectX::XMFLOAT4 UnitW4;

	static DirectX::XMFLOAT4 MinusUnitX4;
	static DirectX::XMFLOAT4 MinusUnitY4;
	static DirectX::XMFLOAT4 MinusUnitZ4;
	static DirectX::XMFLOAT4 MinusUnitW4;

    static DirectX::XMFLOAT3 UnitX3;
    static DirectX::XMFLOAT3 UnitY3;
    static DirectX::XMFLOAT3 UnitZ3;

	static DirectX::XMFLOAT2 UnitX2;
	static DirectX::XMFLOAT2 UnitY2;

	static DirectX::XMFLOAT4 Zero4;
	static DirectX::XMFLOAT3 Zero3;
	static DirectX::XMFLOAT2 Zero2;

	static XMMATRIX PositiveXViewMatrix;
	static XMMATRIX NegativeXViewMatrix;

	static XMMATRIX PositiveYViewMatrix;
	static XMMATRIX NegativeYViewMatrix;

	static XMMATRIX PositiveZViewMatrix;
	static XMMATRIX NegativeZViewMatrix;

	static UINT CalcMipLevel(UINT nSize);
};