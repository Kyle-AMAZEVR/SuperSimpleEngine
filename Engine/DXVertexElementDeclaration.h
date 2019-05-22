

#pragma once

class ENGINE_API DXVertexElementDeclaration
{
public:
    // POSTION, COLOR
    static D3D11_INPUT_ELEMENT_DESC PositionColor[2];

    // POSITION, TEXCOORD, NORMAL
    static D3D11_INPUT_ELEMENT_DESC PositionTexcoordNormal[3];

    
};