

#pragma once



class ENGINE_API DXVertexElementDeclaration
{
public:
    // POSTION, COLOR
    static const D3D11_INPUT_ELEMENT_DESC PositionColor[2];    

    // POSITION, NORMAL, TEXCOORD
    static const D3D11_INPUT_ELEMENT_DESC PositionNormalTexcoord[3];
    
};

