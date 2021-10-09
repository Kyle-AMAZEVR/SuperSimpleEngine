
#include "SSManagerBase.h"

SSManagerBase::SSManagerBase()
{
	mManagerList.push_back(this);
}

void SSManagerBase::InitializeManagers()
{
	for(auto* manager : mManagerList)
	{
		manager->Initialize();
	}
}

void SSManagerBase::ShutdownManagers()
{
	for(auto* manager : mManagerList)
	{
		manager->Shutdown();
	}

	mManagerList.clear();
}


std::vector<SSManagerBase*>  SSManagerBase::mManagerList;