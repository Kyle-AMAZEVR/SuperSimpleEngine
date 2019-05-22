#pragma once


class ENGINE_API DXShader
{
public:
    virtual ~DXShader() {}
protected:
    virtual bool CompileFromFile(std::wstring filepath) { return true; }
	
    void PrintCompileError(ID3D10Blob* errorMsg);
};

// vertex shader
class ENGINE_API DXVertexShader : public DXShader
{
public:
    DXVertexShader() = default;
    virtual ~DXVertexShader();
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11VertexShader* GetShader() { return mVertexShader; }
protected:
    ID3D11VertexShader* mVertexShader = nullptr;
};


// pixel shader
class ENGINE_API DXPixelShader : public DXShader
{
    DXPixelShader() = default;
    virtual ~DXPixelShader();
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11PixelShader* GetShader() { return mPixelShader; }
protected:
    ID3D11PixelShader* mPixelShader = nullptr;
};
