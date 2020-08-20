#pragma once


class COMMON_API SSGameScene
{
public:
	virtual void Tick(float deltaSeconds);

	virtual void InitializeScene();

	virtual void DestroyScene();
	
	virtual void AddGameObject(class SSGameObject* gameObject);

	virtual void DrawScene(float deltaSec);
protected:
	
	std::map<UINT, class SSGameObject*> mSceneObjectMap;	
};