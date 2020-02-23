

#pragma once

#include "SSCommon.h"

class COMMON_API SSVertexElementDeclaration
{
public:
    // POSTION, COLOR
    static D3D11_INPUT_ELEMENT_DESC PositionColor[2];    

    // POSITION, NORMAL, TEXCOORD
    static D3D11_INPUT_ELEMENT_DESC PositionNormalTexcoord[3];
    
	// POSITION, COLOR
    static D3D12_INPUT_ELEMENT_DESC PositionColorElementDesc[2];

    // POSITION, NORMAL, TEXCOORD
    static D3D12_INPUT_ELEMENT_DESC PositionNormalTexcoordElementDesc[3];
};

