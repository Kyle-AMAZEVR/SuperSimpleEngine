#pragma once

#include "Singleton.h"
#include <map>

class ENGINE_API SSSceneObjectManager : Singleton<SSSceneObjectManager>
{
public:
	void AddSceneObject(class SSSceneObject* object);
	void RemoveSceneObject(UINT id);
	void Tick(float delta);
	
protected:	
	friend class SSSceneObject;
	UINT IssueObjectId() { return mNextObjectId++; }	
	UINT mNextObjectId = 1;
	std::map<UINT, SSSceneObject*> mSceneObjectMap;
};