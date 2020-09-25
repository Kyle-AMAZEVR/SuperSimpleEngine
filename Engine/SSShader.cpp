#include "Core.h"
#include "SSShader.h"
#include "SSDX11Engine.h"
#include "SSVertexElementDeclaration.h"
#include <filesystem>
#include <map>
#include "SSDXTranslator.h"
#include "SSTexture2D.h"

#pragma region DXShaderImplementation

void SSShader::PrintCompileError(ID3DBlob* errorMessage)
{	
    auto buffSize = errorMessage->GetBufferSize();

    const char* errorMsgPtr = (const char*) errorMessage->GetBufferPointer();    

	char* compileErrBuffer = new char[buffSize + 1];

	strcpy_s(compileErrBuffer, buffSize, errorMsgPtr);

    OutputDebugStringA("============= Shader Compile Error =============\n");
	OutputDebugStringA(compileErrBuffer);
    OutputDebugStringA("============= Shader Compile Error =============\n");

	delete[] compileErrBuffer;
}

ID3D11Buffer* SSShader::GetConstantBuffer(std::string bufferName)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
        return mConstantBufferMap[bufferName]->GetDX11BufferPointer();
    }

    return nullptr;
}

void SSShader::ReflectCompiledShader(ID3D11ShaderReflection* shaderReflection)
{
	check(mShaderBuffer != nullptr);
	check(shaderReflection != nullptr);
	
	D3D11_SHADER_DESC shaderDescription;
	shaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{		
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = new SSGenericConstantBuffer(constantBuffer, i);
	}
	// @ end

	//
	for (unsigned int i = 0; i < shaderDescription.BoundResources; ++i)
	{
		D3D11_SHADER_INPUT_BIND_DESC desc;
		shaderReflection->GetResourceBindingDesc(i, &desc);

		if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D_SIT_TEXTURE)
		{
			mTextureMap[desc.Name] = desc.BindPoint;
		}
		else if (desc.Type == D3D_SHADER_INPUT_TYPE::D3D10_SIT_SAMPLER)
		{
			mSamplerMap[desc.Name] = desc.BindPoint;
		}
	}
}

std::vector<std::string> SSShader::GetSamplerNames() 
{
	std::vector<std::string> result;
	for (auto& k : mSamplerMap)
	{
		result.push_back(k.first);
	}
	return result;
}



#pragma endregion


void SSVertexShader::Destroy()
{
	for (auto& kvp : mConstantBufferMap)
	{
		if (kvp.second)
		{
			kvp.second->Destroy();
		}
	}

	ReleaseCOM(mVertexShader);
}

void SSVertexShader::CreateInputLayout(ID3D11ShaderReflection* shaderReflection)
{
    auto* dxDevice = SSDX11Engine::Get().GetDevice();
    
    check(dxDevice != nullptr);    
	check(shaderReflection != nullptr);

	D3D11_SHADER_DESC shaderDescription;
	shaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = shaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = new SSGenericConstantBuffer(constantBuffer, i);
	}

	// @input layout creation
	UINT inputParamCount = shaderDescription.InputParameters;
	D3D11_INPUT_ELEMENT_DESC* inputDescriptions = new D3D11_INPUT_ELEMENT_DESC[inputParamCount];
	UINT byteOffset = 0;

	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		auto* dxDevice = SSDX11Engine::Get().GetDevice();

		check(dxDevice != nullptr);

		D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
		shaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = SSDXTranslator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;
		inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		inputDescriptions[i].SemanticIndex = 0;
		inputDescriptions[i].InstanceDataStepRate = 0;
		inputDescriptions[i].InputSlot = 0;
		inputDescriptions[i].AlignedByteOffset = byteOffset;

		byteOffset += SSDXTranslator::GetDXGIFormatByteSize(format);
	}

	HR(dxDevice->CreateInputLayout(inputDescriptions, inputParamCount, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));

	delete [] inputDescriptions;
}

bool SSVertexShader::CompileFromFile(std::wstring filepath, std::vector<std::pair<std::string_view, std::string_view>> defines)
{
    ID3DBlob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);

        ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = SSDX11Engine::Get().GetDevice();

    HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));


    ID3D11ShaderReflection* vertexShaderReflection = nullptr;
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));

    // @constant buffer reflection
    ReflectCompiledShader(vertexShaderReflection);

    // @input layout creation
    CreateInputLayout(vertexShaderReflection);

    return true;
}

 bool SSVertexShader::CompileFromFile(std::wstring filepath)
 {
    ID3DBlob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
		PrintCompileError(errorMsg);
    	
		ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = SSDX11Engine::Get().GetDevice();
    
	HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));
	

    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  
	
	// @constant buffer reflection
	ReflectCompiledShader(vertexShaderReflection);
    
	// @input layout creation
	CreateInputLayout(vertexShaderReflection);	

    return true;
 }


void SSVertexShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture)
{
	check(mTextureMap.count(name) > 0);
	check(deviceContext != nullptr);	

	UINT slotIndex = mTextureMap[name];	

	deviceContext->VSSetShaderResources(slotIndex, 1, texture->GetShaderResourceViewRef());
}

void SSVertexShader::SetSampler(ID3D11DeviceContext* deviceContext, std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];

	check(deviceContext != nullptr);

	deviceContext->VSSetSamplers(slotIndex, 1, &sampler);
}

 void SSVertexShader::SetSampler(std::string name, ID3D11SamplerState* sampler)
 { 	 
	 auto* dxDeviceContext = SSDX11Engine::Get().GetImmediateDeviceContext();
	 
	 SetSampler(dxDeviceContext, name, sampler);
 }

#pragma region PixelShader

 void SSPixelShader::Destroy()
 {
	 for (auto& kvp : mConstantBufferMap)
	 {
		 if (kvp.second)
		 {
			 kvp.second->Destroy();
		 }
	 }
	 ReleaseCOM(mPixelShader);
 }

bool SSPixelShader::CompileFromFile(std::wstring filepath, std::vector<std::pair<std::string_view, std::string_view>> defines)
{
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    std::vector<D3D_SHADER_MACRO> macros;

    std::vector<std::pair<std::string,std::string>> tempDefines;

    for(auto& [name, define] : defines)
    {
        std::string macroName{name};
        std::string macroDefine{define};

        tempDefines.push_back({macroName, macroDefine});
    }

    for(auto& [name, define] : tempDefines)
    {
        D3D_SHADER_MACRO macro{name.data(), define.data()};
        macros.push_back(macro);
    }

    // null terminate , otherwise crashes
    macros.push_back({nullptr,nullptr});

    D3DCompileFromFile(filepath.c_str(), macros.data(), nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);
        return false;
    }

    auto* dxDevice = SSDX11Engine::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

    // @constant buffer reflection
    ID3D11ShaderReflection* pixelShaderReflection = nullptr;
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

    ReflectCompiledShader(pixelShaderReflection);

    return true;
}

bool SSPixelShader::CompileFromFile(std::wstring filepath)
{    
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);		
        return false;
    }

    auto* dxDevice = SSDX11Engine::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

	// @constant buffer reflection
	ID3D11ShaderReflection* pixelShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	ReflectCompiledShader(pixelShaderReflection);

    return true;
}


void SSPixelShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSTexture2DBase* texture)
{
	check(deviceContext != nullptr);

	if (mTextureMap.count(name) > 0)
	{
		UINT slotIndex = mTextureMap[name];

		deviceContext->PSSetShaderResources(slotIndex, 1, texture->GetShaderResourceViewRef());
	}
}

void SSPixelShader::SetTextureAsNull(ID3D11DeviceContext* deviceContext, std::string name)
{
	check(deviceContext != nullptr);

	if (mTextureMap.count(name) > 0)
	{
		UINT slotIndex = mTextureMap[name];		

		ID3D11ShaderResourceView* nullResourceView = nullptr;

		deviceContext->PSSetShaderResources(slotIndex, 1, &nullResourceView);
	}
}

void SSPixelShader::SetTextureAsNull(std::string name)
{
	if (mTextureMap.count(name) > 0)
	{
		UINT slotIndex = mTextureMap[name];

		auto* dxDeviceContext = SSDX11Engine::Get().GetImmediateDeviceContext();

		ID3D11ShaderResourceView* nullResourceView = nullptr;

		dxDeviceContext->PSSetShaderResources(slotIndex, 1, &nullResourceView);
	}
}


void SSPixelShader::SetSampler(std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];

	auto* dxDeviceContext = SSDX11Engine::Get().GetImmediateDeviceContext();

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

void SSPixelShader::SetSampler(ID3D11DeviceContext* dxDeviceContext, std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];	

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

#pragma endregion