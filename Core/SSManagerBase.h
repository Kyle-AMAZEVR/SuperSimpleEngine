#pragma once

#include "Singleton.h"
#include <vector>

class CORE_API SSManagerBase
{
public:
	SSManagerBase();

	virtual void Initialize() {}
	virtual void Shutdown(){}

	static void InitializeManagers();
	static void ShutdownManagers();

protected:
	static std::vector<SSManagerBase*> mManagerList;
};
