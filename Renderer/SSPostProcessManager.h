#pragma once
#include "Singleton.h"
#include "SSManagerBase.h"
#include "SSPostProcess.h"


class SSPostProcessManager : public Singleton<SSPostProcessManager> , public SSManagerBase
{
public:

	virtual void Initialize() override;
	virtual void Shutdown() override;

	void OnResize(UINT with, UINT height);

protected:

	std::vector<SSPostProcess*> mPostProcessList;
};
