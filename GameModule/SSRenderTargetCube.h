

#pragma once


#include "SSRenderThreadObject.h"

class GAMEMODULE_API SSRenderTargetCube : public SSRenderThreadObject
{
public:
	SSRenderTargetCube();
	
protected:	
	void CreateVertexData() override;
};