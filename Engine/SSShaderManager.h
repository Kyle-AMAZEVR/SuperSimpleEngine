#pragma once

#include "Singleton.h"
#include "SSManagerBase.h"

class ENGINE_API SSShaderManager : public Singleton<SSShaderManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;

protected:
	
};


