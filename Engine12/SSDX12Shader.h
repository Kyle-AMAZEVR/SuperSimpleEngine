
#pragma once

class SSDX12ShaderCompileOption
{
public:
	SSDX12ShaderCompileOption();
protected:
	std::string mEntryPoint;
	std::string mTargetName;
	D3D_SHADER_MACRO mMacro;
};



class SSDX12Shader
{
public:
	ID3DBlob* GetCompiledShader();	

protected:
	virtual void ReflectCompiledShader(ID3D12ShaderReflection* reflection);
	virtual void CompileFromFile(ID3D12Device* device, std::wstring filepath);	

	ComPtr<ID3DBlob> mShaderBuffer = nullptr;
};

class SSVertexShader : public SSDX12Shader
{
public:
	virtual void CompileFromFile(ID3D12Device* device, std::wstring filepath) override;

	void CreateInputLayout(ID3D12Device* device, ID3D12ShaderReflection* shaderReflection);

protected:

};