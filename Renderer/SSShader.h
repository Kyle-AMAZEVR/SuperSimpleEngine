#pragma once

#include "SSRendererModulePCH.h"
#include "SSRenderThreadObject.h"
#include "SSDX11ConstantBuffer.h"
#include "SSName.h"
#include "wrl/client.h"
#include <map>

using namespace std;
using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSCompileContext
{
public:
    vector<pair<string_view, string_view>> MacroDefines;
    vector<string_view> InstancedAttributes;
};

//class ENGINE_API SSShader : public SSRenderThreadObject
class DX11RENDERER_API SSShader :  public SSRenderThreadResidentObject
{
public:
	virtual ~SSShader(){}

    template<class T>
    void SetConstantBufferData(std::string bufferName, const T& data);	

    virtual ID3D11Buffer* GetConstantBuffer(std::string bufferName);

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* textrue ){}	
	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name){}
	virtual void SetTextureAsNull(std::string name){}
	
	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) {}
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) {}

	std::vector<std::string> GetSamplerNames();

	inline std::map<SSName, SSGenericConstantBuffer*> GetConstantBufferMap() { return mConstantBufferMap; }

	ID3DBlob* GetCompiledShader() { return mShaderBuffer.Get(); }

    virtual bool CompileFromFile(std::wstring filepath) { return true; }

    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) {return true;}
protected:
	
	virtual void ReflectCompiledShader(ID3D11ShaderReflection* reflection);

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
class DX11RENDERER_API SSVertexShader : public SSShader
{
public:
    SSVertexShader() = default;
	virtual ~SSVertexShader();	
    virtual bool CompileFromFile(std::wstring filepath) override;
    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) override;
    ID3D11VertexShader* GetShader() { return mVertexShader; } 
    ID3D11InputLayout* GetInputLayout() { return mInputLayout; }

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* texture) override;

	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) override;	
	
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;
protected:	
    void CreateInputLayout(ID3D11ShaderReflection* shaderReflection, const SSCompileContext& context);
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;


};

template<class T>
void SSVertexShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{	
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(deviceContext, data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->VSSetConstantBuffers(bufferIndex, 1, &mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}

// pixel shader
class DX11RENDERER_API SSPixelShader : public SSShader
{
public:
    SSPixelShader() = default;
	virtual ~SSPixelShader();	
    virtual bool CompileFromFile(std::wstring filepath) override;
    virtual bool CompileFromFile(std::wstring filepath, const SSCompileContext& context) override;

	ID3D11PixelShader* GetShader()
	{
		return mPixelShader.PixelShaderDX11Ptr;
	}	

	template<class T>
	void SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data);

	virtual void SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name)override;
	virtual void SetTexture(ID3D11DeviceContext* deviceContext, std::string name, class SSDX11Texture2D* texture) override;
	virtual void SetTextureAsNull(std::string name) override;
	virtual void SetSampler(std::string name, ID3D11SamplerState* sampler) override;
	virtual void SetSampler(ID3D11DeviceContext* device, std::string name, ID3D11SamplerState* sampler) override;
protected:

	union PixelShaderPtr
	{
		void* PixelShaderVoidPtr;
		ID3D11PixelShader* PixelShaderDX11Ptr;
	} ;

	PixelShaderPtr mPixelShader;
};

template<class T>
void SSPixelShader::SetConstantBufferData(ID3D11DeviceContext* deviceContext, std::string bufferName, const T& data)
{
	if (mConstantBufferMap.count(bufferName) > 0)
	{
		mConstantBufferMap[bufferName]->SetBufferData<T>(deviceContext, data);
		UINT bufferIndex = mConstantBufferMap[bufferName]->GetBufferIndex();
		deviceContext->PSSetConstantBuffers(
			bufferIndex,
			1,
			&mConstantBufferMap[bufferName]->GetBufferPointerRef());
	}
}
