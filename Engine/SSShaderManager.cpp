
#include "Core.h"
#include "SSShaderManager.h"
#include "SSName.h"
#include "SSShader.h"

void SSShaderManager::Initialize()
{
	//
	WIN32_FIND_DATA fdd;
	HANDLE hFind = FindFirstFile("./Shader", &fdd);


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