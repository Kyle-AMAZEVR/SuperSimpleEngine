
#pragma once

#include "SSObjectBase.h"

class GAMEMODULE_API SSRenderThreadObject : public SSObjectBase
{
public:
	SSRenderThreadObject();
	virtual ~SSRenderThreadObject();
	virtual void Destroy(){}
	
};

class GAMEMODULE_API SSRenderThreadResidentObject
{
public:
	SSRenderThreadResidentObject();
	virtual ~SSRenderThreadResidentObject();
};
