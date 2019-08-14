
#include "Core.h"
#include "SSShaderManager.h"
#include "SSName.h"
#include "SSShader.h"

#include <filesystem>

void SSShaderManager::Initialize()
{
	for (auto& f : std::filesystem::directory_iterator("./Shader"))
	{
		std::string filename;
		std::wstring wfilename = f.path().c_str();
		filename.assign(wfilename.begin(), wfilename.end());

		if (filename.find(".vs") != std::string::npos)
		{
			std::shared_ptr<SSVertexShader> vs = std::make_shared<SSVertexShader>();
			if (vs->CompileFromFile(wfilename) == true)
			{
				mVertexShaderMap[filename] = vs;
			}
		}

		else if (filename.find(".ps") != std::string::npos)
		{
			std::shared_ptr<SSPixelShader> ps = std::make_shared<SSPixelShader>();
			if (ps->CompileFromFile(wfilename) == true)
			{
				mPixelShaderMap[filename] = ps;
			}
		}
	}


}


void SSShaderManager::Shutdown()
{
	//
	for (auto& kvp : mVertexShaderMap)
	{
		kvp.second->Destroy();
	}

	for (auto& kvp : mPixelShaderMap)
	{
		kvp.second->Destroy();
	}
}


std::shared_ptr<SSVertexShader> SSShaderManager::GetVertexShader(SSName name)
{
	return mVertexShaderMap[name];
}

std::shared_ptr<SSPixelShader> SSShaderManager::GetPixelShader(SSName name)
{
	return mPixelShaderMap[name];
}