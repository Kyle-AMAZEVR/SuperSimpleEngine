#pragma once

#include <map>

class GAMEMODULE_API SSGameObjectManager
{
public:
	void AddGameObject(class SSGameObject* object);
	void RemoveGameObject(UINT id);
	void Tick(float delta);

	static SSGameObjectManager& Get();
	static SSGameObjectManager* GetPtr();

	const std::map<UINT, SSGameObject*>& GetGameObjectMap() { return mGameObjectMap; }
	
protected:	
	friend class SSGameObject;
	UINT IssueObjectId() { return mNextObjectId++; }	
	UINT mNextObjectId = 1;
	std::map<UINT, SSGameObject*> mGameObjectMap;
	static SSGameObjectManager* mInstance;
};