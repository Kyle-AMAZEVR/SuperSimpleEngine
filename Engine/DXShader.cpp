
#include "Core.h"
#include "DXShader.h"
#include "DXEngine.h"
#include "DXVertexElementDeclaration.h"
#include <filesystem>
#include <fstream>
#include "Templates.h"

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