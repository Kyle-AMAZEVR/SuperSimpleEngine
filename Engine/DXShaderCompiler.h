
#pragma once

#include "Singleton.h"
#include <string>

class ENGINE_API DXShaderCompiler : public Singleton<DXShaderCompiler>
{
public:
    bool CompileVertexShader(std::wstring filename); 
    bool CompilePixelShader(std::wstring filename);

protected:
    
};