
#include "Core.h"
#include "DXShaderCompiler.h"
#include <filesystem>



bool DXShaderCompiler::CompileVertexShader(std::wstring filename)
{    
    ID3DBlob* vertexShaderBuffer = nullptr;
    ID3DBlob* errorMsg = nullptr;

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
    ID3DBlob* pixelShaderBuffer = nullptr;
    ID3DBlob* errorMsg = nullptr;

    check(std::filesystem::exists(filename));

    D3DCompileFromFile(filename.c_str(), nullptr, nullptr, "PSMain", "ps_5_0", 0, 0, &pixelShaderBuffer, &errorMsg);

    if(errorMsg != nullptr)
    {
        return false;
    }

    return true;    
}
