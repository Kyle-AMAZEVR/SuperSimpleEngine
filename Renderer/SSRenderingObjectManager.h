#pragma once


#include <map>
#include "Singleton.h"

class DX11RENDERER_API SSRenderingObjectManager : public Singleton<SSRenderingObjectManager>
{
public:
	void Tick(float deltaSeconds);
	const std::map<UINT, class SSRenderingObject*>& GetRenderingObjectMap() { return mRenderingObjectMap; }
	
private:
	std::map<UINT, class SSRenderingObject*> mRenderingObjectMap;
};