#pragma once

class ENGINE_API DXShader
{
public:

};

class ENGINE_API DXVertexShader : public DXShader
{
public:

protected:
    ID3D11VertexShader* mVertexShader;
};