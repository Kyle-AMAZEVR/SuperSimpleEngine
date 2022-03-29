#include "SSRendererModulePCH.h"
#include "SSShader.h"
#include "SSDX11Renderer.h"
#include <filesystem>
#include <map>
#include "SSDXTranslator.h"
#include "SSDX11Texture2D.h"

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


SSVertexShader::~SSVertexShader()
{
	for (auto& kvp : mConstantBufferMap)
	{
		if (kvp.second)
		{
			//kvp.second->Destroy();
		}
	}

	ReleaseCOM(mVertexShader);
}

void SSVertexShader::CreateInputLayout(ID3D11ShaderReflection* shaderReflection, const SSCompileContext& context)
{
    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    
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

	map<std::string, int> semanticIndexMap;

	for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
	{
		auto* dxDevice = SSDX11Renderer::Get().GetDevice();

		check(dxDevice != nullptr);

		D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
		shaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = SSDXTranslator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;

		if(semanticIndexMap.count(inputDesc.SemanticName) > 0)
        {
		    semanticIndexMap[inputDesc.SemanticName]++;
        }
		else
        {
		    semanticIndexMap[inputDesc.SemanticName] = 0;
        }

		// instanced
		string_view vertexAttributeName = inputDesc.SemanticName;
		bool bInstanced = std::find(context.InstancedAttributes.begin(), context.InstancedAttributes.end(), vertexAttributeName) != context.InstancedAttributes.end();
		if(bInstanced)
        {
		    inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_INSTANCE_DATA;
		    inputDescriptions[i].InstanceDataStepRate = 1;
            inputDescriptions[i].AlignedByteOffset = 0;
        }
		else
        {
            inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
            inputDescriptions[i].InstanceDataStepRate = 0;
            inputDescriptions[i].AlignedByteOffset = i == 0 ? 0 : D3D11_APPEND_ALIGNED_ELEMENT;
        }

		inputDescriptions[i].SemanticIndex = semanticIndexMap[inputDesc.SemanticName];
		inputDescriptions[i].InputSlot = 0;

		byteOffset += SSDXTranslator::GetDXGIFormatByteSize(format);
	}

	HR(dxDevice->CreateInputLayout(inputDescriptions, inputParamCount, mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));

	delete [] inputDescriptions;
}

bool SSVertexShader::CompileFromFile(std::wstring filepath, const SSCompileContext& compileContext)
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

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();

    HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));


    ID3D11ShaderReflection* vertexShaderReflection = nullptr;
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));

    // @constant buffer reflection
    ReflectCompiledShader(vertexShaderReflection);

    // @input layout creation
    CreateInputLayout(vertexShaderReflection, compileContext);

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

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    
	HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));
	

    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  
	
	// @constant buffer reflection
	ReflectCompiledShader(vertexShaderReflection);

	SSCompileContext dummy;

	// @input layout creation
	CreateInputLayout(vertexShaderReflection, dummy);

    return true;
 }


void SSVertexShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
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
	 auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();
	 
	 SetSampler(dxDeviceContext, name, sampler);
 }

#pragma region PixelShader

 SSPixelShader::~SSPixelShader() 
 {
	 for (auto& kvp : mConstantBufferMap)
	 {
		 if (kvp.second)
		 {
			// kvp.second->Destroy();
		 }
	 }
	 ReleaseCOM(mPixelShader.PixelShaderDX11Ptr);
 }

bool SSPixelShader::CompileFromFile(std::wstring filepath, const SSCompileContext& context)
{
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    std::vector<D3D_SHADER_MACRO> macros;

    for(auto& [name, define] : context.MacroDefines)
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

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader.PixelShaderDX11Ptr));

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

    auto* dxDevice = SSDX11Renderer::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader.PixelShaderDX11Ptr));

	// @constant buffer reflection
	ID3D11ShaderReflection* pixelShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	ReflectCompiledShader(pixelShaderReflection);

    return true;
}


void SSPixelShader::SetTexture(ID3D11DeviceContext* deviceContext, std::string name, SSDX11Texture2D* texture)
{
	check(deviceContext != nullptr);

	if (mTextureMap.count(name) > 0 && texture != nullptr)
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

		auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();

		ID3D11ShaderResourceView* nullResourceView = nullptr;

		dxDeviceContext->PSSetShaderResources(slotIndex, 1, &nullResourceView);
	}
}


void SSPixelShader::SetSampler(std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];

	auto* dxDeviceContext = SSDX11Renderer::Get().GetImmediateDeviceContext();

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

void SSPixelShader::SetSampler(ID3D11DeviceContext* dxDeviceContext, std::string name, ID3D11SamplerState* sampler)
{
	UINT slotIndex = mSamplerMap[name];	

	dxDeviceContext->PSSetSamplers(slotIndex, 1, &sampler);
}

#pragma endregion