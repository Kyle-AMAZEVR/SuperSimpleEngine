#pragma once

#include "SSGameObject.h"


class GAMEMODULE_API SSCube : public SSGameObject
{
public:
	SSCube();	

protected:	
	void CreateRenderingData() override;		
};