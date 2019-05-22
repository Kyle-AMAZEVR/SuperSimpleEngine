
#include "Core.h"
#include "DXShader.h"
#include "DXEngine.h"
#include "DXVertexElementDeclaration.h"
#include <filesystem>

//@ vertex shader implementation
 DXVertexShader::~DXVertexShader()
 {
     ReleaseCOM(mVertexShader);
 }

 bool DXVertexShader::CompileFromFile(std::wstring filepath)
 {
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertexShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        return false;
    }

    auto* dxDevice = DXEngine::Get().GetDevice();
    HR(dxDevice->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &mVertexShader));

    return true;
 }

DXPixelShader::~DXPixelShader()
{
    ReleaseCOM(mPixelShader);
}

bool DXPixelShader::CompileFromFile(std::wstring filepath)
{
    ID3D10Blob* pixelShaderBuffer = nullptr;
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filepath));

    D3DCompileFromFile(filepath.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixelShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        return false;
    }

    auto* dxDevice = DXEngine::Get().GetDevice();
    HR(dxDevice->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &mPixelShader));

    return true;
}