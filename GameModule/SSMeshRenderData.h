#pragma once

#include "SSVertexTypes.h"
#include <vector>

enum class EVertexType : unsigned char
{
	VT_PNT, // Position, Normal, Texture
	VT_PNTT, // Position, Normal, Texture , Tangent
};

struct SSMeshRenderData
{
public:
	EVertexType VertexType = EVertexType::VT_PNT;
	std::vector<VT_PositionNormalTexcoord> PNT_VertexData;
	std::vector<VT_PositionNormalTexcoordTangent> PNTT_VertexData;
	bool bHasIndexData = false;
	std::vector<unsigned int> IndexData;
};

struct SSMeshElementData
{
public:
	unsigned int nStartIndex = 0;
	unsigned int nEndIndex = 0;
};