#pragma once

#include "SSObjectBase.h"
#include <map>

class RENDERCORE_API SSRenderingObjectManager : public Singleton<SSRenderingObjectManager>
{
public:
	SSRenderingObjectManager();

	void SetPendingObjects(std::map<UINT, SSObjectBase*> objectMap);

	void UpdateObjects();
	
	const std::map<UINT, class SSRenderingObject*>& GetRenderingObjectMap() { return mRenderingObjectMap; }
	
private:
	std::map<UINT, class SSRenderingObject*> mRenderingObjectMap;
	std::map<UINT, class SSObjectBase*> mPendingObjectMap;

	CRITICAL_SECTION mCriticalSection;

};