
#include "SSRendererModulePCH.h"
#include "SSDX11PostProcess.h"
#include "SSPostProcessManager.h"


void SSPostProcessManager::Initialize()
{
	
}

void SSPostProcessManager::Shutdown()
{
	
}


void SSPostProcessManager::OnResize(UINT width, UINT height)
{
	for(auto& postprocess : mPostProcessList)
	{
		postprocess->OnResize(width, height);
	}
}
