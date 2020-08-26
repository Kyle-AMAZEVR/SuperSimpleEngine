#pragma once

#include "SSGameObject.h"


class GAMEMODULE_API SSCube : public SSGameObject
{
public:
	SSCube();	
	void Tick(float delta) override;
protected:	
	void CreateRenderingData() override;		
};