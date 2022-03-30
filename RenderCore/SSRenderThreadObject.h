
#pragma once

#include "SSRenderCore.h"
#include "SSObjectBase.h"

class RENDERCORE_API SSRenderThreadObject : public SSObjectBase
{
public:
	SSRenderThreadObject();
	virtual ~SSRenderThreadObject();
	virtual void Destroy(){}
	
};