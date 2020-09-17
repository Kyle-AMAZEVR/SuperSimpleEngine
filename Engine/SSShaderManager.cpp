
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

		std::wstring filepath = f.path().c_str();
		std::wstring wfilename = f.path().filename().c_str();

		filename.assign(wfilename.begin(), wfilename.end());		

		if (filename.find(".vs") != std::string::npos)
		{
			std::shared_ptr<SSVertexShader> vs = std::make_shared<SSVertexShader>();

			if (vs->CompileFromFile(filepath) == true)
			{
				mVertexShaderMap[filename] = vs;
			}
			else
			{
				check(false);
			}
		}
		else if (filename.find(".ps") != std::string::npos)
		{
		    std::vector<D3D_SHADER_MACRO> macros;


			std::shared_ptr<SSPixelShader> ps = std::make_shared<SSPixelShader>();
			if (ps->CompileFromFile(filepath, macros) == true)
			{
				mPixelShaderMap[filename] = ps;
			}
			else
			{
				check(false);
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
	check(mVertexShaderMap.count(name) > 0);
	return mVertexShaderMap[name];
}

std::shared_ptr<SSPixelShader> SSShaderManager::GetPixelShader(SSName name)
{
	check(mPixelShaderMap.count(name) > 0);
	return mPixelShaderMap[name];
}