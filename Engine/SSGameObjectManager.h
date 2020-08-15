#pragma once

#include "Singleton.h"
#include <map>

class ENGINE_API SSGameObjectManager : Singleton<SSGameObjectManager>
{
public:
	void AddSceneObject(class SSGameObject* object);
	void RemoveSceneObject(UINT id);
	void Tick(float delta);
	
protected:	
	friend class SSGameObject;
	UINT IssueObjectId() { return mNextObjectId++; }	
	UINT mNextObjectId = 1;
	std::map<UINT, SSGameObject*> mSceneObjectMap;
};