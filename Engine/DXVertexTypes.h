
#pragma once

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
    T1 VertexAttribute1;
    T2 VertexAttribute2;
};

template<class T1, class T2, class T3>
struct VertexType<T1, T2, T3>
{    
    T1 VertexAttribute1;
    T2 VertexAttribute2;
    T3 VertexAttribute3;
};

template<class T1, class T2, class T3, class T4>
struct VertexType<T1, T2, T3, T4>
{    
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

using VT_PositionColor = VertexType<DirectX::XMFLOAT3, DirectX::XMFLOAT4>;




