#pragma once

#include "SSRenderThreadObject.h"
#include "DXConstantBuffer.h"
#include "SSName.h"
#include "wrl/client.h"
#include <map>

using Microsoft::WRL::ComPtr;

class ENGINE_API SSShader : public SSRenderThreadObject
{
public:
	virtual ~SSShader(){}

    template<class T>
    void SetConstantBufferData(std::string bufferName, const T& data);	

    virtual ID3D11Buffer* GetConstantBuffer(std::string bufferName);

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSTexture2DBase* textrue ){}	
	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name){}
	virtual void SetTextureAsNull(std::string name){}
	
	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) {}
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) {}

	std::vector<std::string> GetSamplerNames();

	inline std::map<SSName, SSGenericConstantBuffer*> GetConstantBufferMap() { return mConstantBufferMap; }

	ID3DBlob* GetCompiledShader() { return mShaderBuffer.Get(); }

protected:
	
	virtual void ReflectCompiledShader(ID3D11ShaderReflection* reflection);
    virtual bool CompileFromFile(std::wstring filepath) { return true; }	
    void PrintCompileError(ID3DBlob* errorMsg);
    
	ComPtr<ID3DBlob> mShaderBuffer = nullptr;

    //std::map<std::string, SSGenericConstantBuffer*> mConstantBufferMap;
	std::map<SSName, SSGenericConstantBuffer*> mConstantBufferMap;
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
	virtual void Destroy() override;
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11VertexShader* GetShader() { return mVertexShader; } 
    ID3D11InputLayout* GetInputLayout() { return mInputLayout; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSTexture2DBase* texture) override;

	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) override;	
	
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
	virtual void Destroy() override;
    virtual bool CompileFromFile(std::wstring filepath) override;
    ID3D11PixelShader* GetShader() { return mPixelShader; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name)override;
	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSTexture2DBase* texture) override;
	virtual void SetTextureAsNull(std::string name) override;
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
