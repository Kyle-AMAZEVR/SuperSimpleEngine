


#include "Core.h"
#include "DXShader.h"
#include "DXEngine.h"
#include "DXVertexElementDeclaration.h"
#include <filesystem>
#include <fstream>
#include "Templates.h"
#include "Translator.h"


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


//@ vertex shader implementation
 DXVertexShader::~DXVertexShader()
 {
     ReleaseCOM(mVertexShader);
 }

ID3D11InputLayout* DXVertexShader::CreateInputLayout()
{
    auto* dxDevice = DXEngine::Get().GetDevice();    
    
    check(dxDevice != nullptr);

    ID3D11InputLayout* resultInputLayout = nullptr;

    auto inputDescElementLength = sizeof_array(DXVertexElementDeclaration::PositionColor);

    HR(dxDevice->CreateInputLayout(DXVertexElementDeclaration::PositionColor, 
    inputDescElementLength, 
    mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), &resultInputLayout)); 


    return resultInputLayout;
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

    // constant buffer reflection
    ID3D11ShaderReflection* vertexShaderReflection = nullptr;    
    HR(D3DReflect(mShaderBuffer->GetBufferPointer(), mShaderBuffer->GetBufferSize(), IID_ID3D11ShaderReflection, (void**) &vertexShaderReflection));  

    D3D11_SHADER_DESC shaderDescription;
    vertexShaderReflection->GetDesc(&shaderDescription);

    for(unsigned int i = 0; i < shaderDescription.ConstantBuffers; ++i)
    {
        ID3D11ShaderReflectionConstantBuffer* constantBuffer = vertexShaderReflection->GetConstantBufferByIndex(i);
		D3D11_SHADER_BUFFER_DESC bufferDesc;
		constantBuffer->GetDesc(&bufferDesc);        
	
        mConstantBufferMap[bufferDesc.Name] = std::shared_ptr<DXGenericConstantBuffer>(new DXGenericConstantBuffer(constantBuffer));
    }
    //

    
    for (auto i = 0; i < shaderDescription.InputParameters; ++i)
    {
        D3D11_SIGNATURE_PARAMETER_DESC inputDesc;
        vertexShaderReflection->GetInputParameterDesc(i, &inputDesc);     

		Translator::GetVertexShaderInputType(inputDesc);
    }

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

    return true;
}
#pragma endregion