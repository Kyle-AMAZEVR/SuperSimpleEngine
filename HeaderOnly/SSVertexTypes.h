
#pragma once

#include <cstddef>
#include "DirectXMath.h"

template<class... Types>
struct count 
{
    static const std::size_t value = sizeof...(Types);
};

template<class ...T>
struct VertexType
{    
};

template<class T1, class T2>    
struct VertexType<T1, T2>
{    
	VertexType() = default;
    VertexType(const T1& param1, const T2& param2)
    : VertexAttribute1(param1), VertexAttribute2(param2)
    {        
    }

    T1 VertexAttribute1;
    T2 VertexAttribute2;
};

template<class T1, class T2, class T3>
struct VertexType<T1, T2, T3>
{   
	VertexType() = default;
    VertexType(const T1& param1, const T2& param2, const T3& param3) 
    : VertexAttribute1(param1), VertexAttribute2(param2), VertexAttribute3(param3)
    {        
    }

    T1 VertexAttribute1;
    T2 VertexAttribute2;
    T3 VertexAttribute3;
};

template<class T1, class T2, class T3, class T4>
struct VertexType<T1, T2, T3, T4>
{   
	VertexType() = default;
	VertexType(const T1& param1, const T2& param2, const T3& param3, const T4& param4)
		: VertexAttribute1(param1), VertexAttribute2(param2), VertexAttribute3(param3), VertexAttribute4(param4)
	{
	}
    T1 VertexAttribute1;
    T2 VertexAttribute2;
    T3 VertexAttribute3;
    T4 VertexAttribute4;
};

template<class T1, class T2, class T3, class T4, class T5>
struct VertexType<T1, T2, T3, T4, T5>
{    
    T1 VertexAttribute1;
    T2 VertexAttribute2;
    T3 VertexAttribute3;
    T4 VertexAttribute4;
    T5 VertexAttribute5;
};

using VT_Position3Color4 = VertexType<DirectX::XMFLOAT3, DirectX::XMFLOAT4>;
using VT_PositionColor = VertexType<DirectX::XMFLOAT4, DirectX::XMFLOAT3>;
using VT_PositionTexcoord = VertexType<DirectX::XMFLOAT4, DirectX::XMFLOAT2>;
using VT_PositionNormalColor = VertexType<DirectX::XMFLOAT4, DirectX::XMFLOAT3, DirectX::XMFLOAT3>;
using VT_PositionNormalTexcoord = VertexType<DirectX::XMFLOAT4, DirectX::XMFLOAT3, DirectX::XMFLOAT2>;
using VT_PositionNormalTexcoordTangent = VertexType<DirectX::XMFLOAT4, DirectX::XMFLOAT3, DirectX::XMFLOAT2, DirectX::XMFLOAT4>;






