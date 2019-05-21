#pragma once


class ENGINE_API DXShader
{
public:
    virtual ~DXShader() {}
protected:
    
};

class ENGINE_API DXVertexShader : public DXShader
{
public:
    DXVertexShader(){}
    DXVertexShader(const std::wstring filename);
    virtual ~DXVertexShader();    
protected:
    ID3D11VertexShader* mVertexShader = nullptr;
};

