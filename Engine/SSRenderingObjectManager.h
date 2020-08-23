#pragma once

#include <map>
#include "Singleton.h"

class SSRenderingObjectManager : Singleton<SSRenderingObjectManager>
{
public:
	void Tick(float deltaSeconds);

private:
	
};