
#include "Core.h"
#include "SSSceneObject.h"
#include "SSSceneObjectManager.h"

void SSSceneObjectManager::AddSceneObject(SSSceneObject* object)
{
	check(object != nullptr);
	object->mObjectId = IssueObjectId();
	mSceneObjectMap[object->mObjectId] = object;
}