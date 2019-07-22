#include "Core.h"
#include "DXShader.h"
#include "DXEngine.h"
#include "DXVertexElementDeclaration.h"
#include <filesystem>
#include <fstream>
#include <map>
#include "Templates.h"
#include "Translator.h"

#pragma region DXShaderImplementation

void DXShader::PrintCompileError(ID3D10Blob* errorMessage)
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

ID3D11Buffer* DXShader::GetConstantBuffer(std::string bufferName)
{
    if(mConstantBufferMap.count(bufferName) > 0)
    {
        return mConstantBufferMap[bufferName]->GetBufferPointer();
    }

    return nullptr;
}



#pragma endregion


//@ vertex shader implementation
 DXVertexShader::~DXVertexShader()
 {
     ReleaseCOM(mVertexShader);
 }

void DXVertexShader::CreateInputLayout()
{
    auto* dxDevice = DXEngine::Get().GetDevice();    
    
    check(dxDevice != nullptr);    

    auto inputDescElementLength = sizeof_array(DXVertexElementDeclaration::PositionColor);

    HR(dxDevice->CreateInputLayout(DXVertexElementDeclaration::PositionColor, 
    inputDescElementLength, 
    mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));     
}

 bool DXVertexShader::CompileFromFile(std::wstring filepath)
 {
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
		PrintCompileError(errorMsg);
    	
		ReleaseCOM(errorMsg);

        return false;
    }

    auto* dxDevice = DXEngine::Get().GetDevice();
    
	HR(dxDevice->CreateVertexShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));

    // @constant buffer reflection
    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  

    D3D11_SHADER_DESC shaderDescription;
    vertexShaderReflection->GetDesc(&shaderDescription);

    for(unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
    {
        ID3D11ShaderReflectionConstantBuffer* constantBuffer = vertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);        
	
        mConstantBufferMap[bufferDesc.Name] = std::shared_ptr<DXGenericConstantBuffer>(new DXGenericConstantBuffer(constantBuffer, i));
    }
    
	// @input layout creation
	UINT inputParamCount = shaderDescription.InputParameters;
	D3D11_INPUT_ELEMENT_DESC* inputDescriptions = new D3D11_INPUT_ELEMENT_DESC[inputParamCount];
	UINT byteOffset = 0;

    for (UINT i = 0; i < shaderDescription.InputParameters; ++i)
    {
		auto* dxDevice = DXEngine::Get().GetDevice();

		check(dxDevice != nullptr);

        D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
        vertexShaderReflection->GetInputParameterDesc(i, &inputDesc);

		auto format = Translator::GetVertexShaderInputType(inputDesc);
		inputDescriptions[i].SemanticName = inputDesc.SemanticName;
		inputDescriptions[i].Format = format;
		inputDescriptions[i].InputSlotClass = D3D11_INPUT_CLASSIFICATION::D3D11_INPUT_PER_VERTEX_DATA;
		inputDescriptions[i].SemanticIndex = 0;
		inputDescriptions[i].InstanceDataStepRate = 0;
		inputDescriptions[i].InputSlot = 0;
		inputDescriptions[i].AlignedByteOffset = byteOffset;

		byteOffset += Translator::GetDXGIFormatByteSize(format);
    }

	HR(dxDevice->CreateInputLayout(inputDescriptions, inputParamCount,
		mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &mInputLayout));

    return true;
 }

#pragma region PixelShader
DXPixelShader::~DXPixelShader()
{
    ReleaseCOM(mPixelShader);
}

bool DXPixelShader::CompileFromFile(std::wstring filepath)
{    
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &mShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        PrintCompileError(errorMsg);
        return false;
    }

    auto* dxDevice = DXEngine::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

	// @constant buffer reflection
	ID3D11ShaderReflection* pixelShaderReflection = nullptr;
	HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pixelShaderReflection));

	D3D11_SHADER_DESC shaderDescription;
	pixelShaderReflection->GetDesc(&shaderDescription);

	for (unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
	{
		ID3D11ShaderReflectionConstantBuffer* constantBuffer = pixelShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);

		mConstantBufferMap[bufferDesc.Name] = std::shared_ptr<DXGenericConstantBuffer>(new DXGenericConstantBuffer(constantBuffer, i));
	}
    return true;
}
#pragma endregion