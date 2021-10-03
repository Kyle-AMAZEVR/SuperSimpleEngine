
#pragma once
#include "SSRendererModulePCH.h"
#include "SSObjectBase.h"

class DX11RENDERER_API SSRenderThreadObject : public SSObjectBase
{
public:
	SSRenderThreadObject();
	virtual ~SSRenderThreadObject();
	virtual void Destroy(){}
	
};

class DX11RENDERER_API SSRenderThreadResidentObject
{
public:
	SSRenderThreadResidentObject();
	virtual ~SSRenderThreadResidentObject();
};
