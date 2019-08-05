#pragma once
#include "DirectXMath.h"

class ENGINE_API DXMathHelper
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
};