#pragma once

#include "SSObjectBase.h"
#include <map>
#include "Singleton.h"

class DX11RENDERER_API SSRenderingObjectManager : public Singleton<SSRenderingObjectManager>
{
public:
	void SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap);

	void UpdateObjects();
	
	const std::map<UINT, class SSRenderingObject*>& GetRenderingObjectMap() { return mRenderingObjectMap; }
	
private:
	std::map<UINT, class SSRenderingObject*> mRenderingObjectMap;
	std::map<UINT, class SSObjectBase*> mPendingObjectMap;
};