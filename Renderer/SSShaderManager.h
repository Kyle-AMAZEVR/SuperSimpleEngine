#pragma once

#include <memory>
#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSName.h"



class DX11RENDERER_API SSShaderManager : public Singleton<SSShaderManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;

	std::shared_ptr<class SSVertexShader> GetVertexShader(SSName name);
	std::shared_ptr<class SSPixelShader> GetPixelShader(SSName name);
protected:
	std::map<SSName, std::shared_ptr<SSVertexShader>> mVertexShaderMap;
	std::map<SSName, std::shared_ptr<SSPixelShader>> mPixelShaderMap;

	std::map<std::string_view, std::shared_ptr<SSVertexShader>> mVertexShaderMap2;
    std::map<std::string_view, std::shared_ptr<SSPixelShader>> mPixelShaderMap2;
};


