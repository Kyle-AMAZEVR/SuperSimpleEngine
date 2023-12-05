#pragma once

#include <memory>
#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSName.h"



class SSShaderManager : public Singleton<SSShaderManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;

	std::shared_ptr<class SSDX11VertexShader> GetVertexShader(SSName name);
	std::shared_ptr<class SSDX11PixelShader> GetPixelShader(SSName name);
protected:
	std::map<SSName, std::shared_ptr<SSDX11VertexShader>> mVertexShaderMap;
	std::map<SSName, std::shared_ptr<SSDX11PixelShader>> mPixelShaderMap;

	std::map<std::string_view, std::shared_ptr<SSDX11VertexShader>> mVertexShaderMap2;
    std::map<std::string_view, std::shared_ptr<SSDX11PixelShader>> mPixelShaderMap2;
};


