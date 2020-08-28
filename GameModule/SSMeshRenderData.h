#pragma once

#include "SSVertexTypes.h"
#include <vector>
#include "SSName.h"
#include "SSConstantBufferProxy.h"

enum class EVertexType : unsigned char
{
	VT_PNT, // Position, Normal, Texture
	VT_PNTT, // Position, Normal, Texture, Tangent
	VT_PT, //
};

// 
struct SSMeshVertexData
{
public:
	std::vector<VT_PositionNormalTexcoord> PNT_VertexData;
	std::vector<VT_PositionNormalTexcoordTangent> PNTT_VertexData;
	std::vector<unsigned int> IndexData;
	// vertex type
	EVertexType VertexType = EVertexType::VT_PNT;
	D3D_PRIMITIVE_TOPOLOGY PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	bool bHasIndexData = false;
};

// per instance 
struct SSMeshRenderData
{
public:
	//
	SSName VertexShaderName;
	SSName PixelShaderName;	

	//
	std::map<SSName, SSConstantBufferProxy> VSConstantBufferMap;
	std::map<SSName, SSConstantBufferProxy> PSConstantBufferMap;

	std::map<SSName, SSName> VSTextureMap;
	std::map<SSName, SSName> PSTextureMap;
};

struct SSMeshElementData
{
public:
	unsigned int nStartIndex = 0;
	unsigned int nEndIndex = 0;	
};