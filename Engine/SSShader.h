#pragma once

#include "DXRenderResource.h"
#include "DXConstantBuffer.h"
#include <map>

class ENGINE_API SSShader : public DXRenderResource
{
public:
    virtual ~SSShader() {}

    template<class T>
    void SetConstantBufferData(std::string bufferName, const T& data);	

    virtual ID3D11Buffer* GetConstantBuffer(std::string bufferName);

	virtual void SetTexture(std::string name, class SSTexture2D* texture){}
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) {}

	std::vector<std::string> GetSamplerNames();

	inline std::map<std::string, SSGenericConstantBuffer*> GetConstantBufferMap() { return mConstantBufferMap; }

protected:
	
	virtual void ReflectCompiledShader(ID3D11ShaderReflection* reflection);
    virtual bool CompileFromFile(std::wstring filepath) { return true; }	
    void PrintCompileError(ID3D10Blob* errorMsg);
    ID3DBlob* mShaderBuffer = nullptr;

    std::map<std::string, SSGenericConstantBuffer*> mConstantBufferMap;
	std::map<std::string, UINT> mTextureMap;
	std::map<std::string, UINT> mSamplerMap;
};

template<class T>
void SSShader::SetConstantBufferData(std::string bufferName, const T& data)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
        mConstantBufferMap[bufferName]->SetBufferData<T>(data);
    }
}




// vertex shader
class ENGINE_API SSVertexShader : public SSShader
{
public:
    SSVertexShader() = default;
    virtual ~SSVertexShader();
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11VertexShader* GetShader() { return mVertexShader; } 
    ID3D11InputLayout* GetInputLayout() { return mInputLayout; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTexture(std::string name, class SSTexture2D* texture) override;
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;
protected:	
    void CreateInputLayout(ID3D11ShaderReflection* shaderReflection);
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
};


template<class T>
void SSVertexShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{	
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->VSSetConstantBuffers(
			bufferIndex, 
			1, 
			&mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}



// pixel shader
class ENGINE_API SSPixelShader : public SSShader
{
public:
    SSPixelShader() = default;
    virtual ~SSPixelShader();
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11PixelShader* GetShader() { return mPixelShader; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTexture(std::string name, class SSTexture2D* texture) override;
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;
protected:
    ID3D11PixelShader* mPixelShader = nullptr;
};

template<class T>
void SSPixelShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->PSSetConstantBuffers(
			bufferIndex,
			1,
			&mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}
