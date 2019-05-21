
#include "Core.h"
#include "DXShaderCompiler.h"
#include <filesystem>



bool DXShaderCompiler::CompileVertexShader(std::wstring filename)
{    
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filename));

    D3DCompileFromFile(filename.c_str(), nullptr, nullptr, "VSMain", "vs_5_0", 0, 0, &vertexShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        return false;
    }

    return true;
}



bool DXShaderCompiler::CompilePixelShader(std::wstring filename)
{
    ID3D10Blob* pixelShaderBuffer = nullptr;
    ID3D10Blob* errorMsg = nullptr;

    check(std::filesystem::exists(filename));

    D3DCompileFromFile(filename.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixelShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        return false;
    }

    return true;
}
