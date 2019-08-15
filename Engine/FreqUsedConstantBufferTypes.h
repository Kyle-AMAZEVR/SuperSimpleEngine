#pragma once

#include "DirectXMath.h"


struct Transform
{
	DirectX::XMFLOAT4X4 Model;
	DirectX::XMFLOAT4X4 View;
	DirectX::XMFLOAT4X4 Proj;
};

struct MVP
{
	DirectX::XMMATRIX ModelViewProj;
};

 struct alignas(16) CbufferFloat
{
	float value;
};

 struct alignas(16) CbufferInvScreenSize
 {
	 XMFLOAT2 InvScreenSize;
 };

 template<class... T>
struct alignas(16) AlignedCBuffer
 {	 
 };

template<class T1>
struct alignas(16) AlignedCBuffer<T1>
{
	T1 value1;	
};


 template<class T1, class T2>
 struct alignas(16) AlignedCBuffer<T1,T2>
 {
	 T1 value1;
	 T2 value2;
 };

 template<class T1, class T2, class T3>
 struct alignas(16) AlignedCBuffer<T1,T2,T3>
 {
	 T1 value1;
	 T2 value2;
	 T3 value3;
 };

 template<class T1, class T2, class T3, class T4>
 struct alignas(16) AlignedCBuffer<T1,T2,T3,T4>
 {
	 T1 value1;
	 T2 value2;
	 T3 value3;
	 T4 value4;
 };