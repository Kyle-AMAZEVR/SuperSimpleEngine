#pragma once

#include "SSObjectBase.h"
#include <map>
#include "Singleton.h"

class DX11RENDERER_API SSRenderingObjectManager : public Singleton<SSRenderingObjectManager>
{
public:
	void Tick(float deltaSeconds);
	void Tick(const std::map<UINT, SSObjectBase*>& objectMap);
	const std::map<UINT, class SSRenderingObject*>& GetRenderingObjectMap() { return mRenderingObjectMap; }
	
private:
	std::map<UINT, class SSRenderingObject*> mRenderingObjectMap;
};