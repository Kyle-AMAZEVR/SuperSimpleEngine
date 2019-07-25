#pragma once

#include "Singleton.h"
#include <map>

class ENGINE_API SSSceneObjectManager : Singleton<SSSceneObjectManager>
{
public:
	void AddSceneObject(class SSSceneObject* object);	

protected:	
	friend class SSSceneObject;
	UINT IssueObjectId() { return mNextObjectId++; }	
	UINT mNextObjectId = 0;
	std::map<UINT, SSSceneObject*> mSceneObjectMap;
};