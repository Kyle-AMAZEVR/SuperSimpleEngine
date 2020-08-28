#pragma once

#include "SSMeshRenderData.h"

class GAMEMODULE_API SSGameObject
{
public:
	SSGameObject();
	virtual ~SSGameObject();

	virtual void OnAddedScene(){}
	virtual void OnRemovedScene(){}
	
	virtual void GameTheadFirstTick(float deltaSeconds){}
	virtual void RenderThreadFirstTick(float deltaSeconds) {}
	
	virtual void Destroy(){}
	virtual void Draw(ID3D11DeviceContext* deviceContext) {}	
	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material) {}
	virtual void DebugDraw(ID3D11DeviceContext* deviceContext, class SSMaterial* material) {}
		
	virtual const SSMeshRenderData& GetRenderData();
	virtual const SSMeshVertexData& GetVertexData();
	
	
	virtual void Tick(float delta) {}

	virtual DirectX::XMMATRIX GetModelTransform();

	UINT GetId() const { return mObjectId; }

	virtual void SetPosition(float x, float y, float z);
	virtual void SetScale(float x, float y, float z);

	virtual void SetPositionX(float x);
	virtual void SetPositionY(float y);
	virtual void SetPositionZ(float z);

	virtual void SetScaleX(float x);
	virtual void SetScaleY(float y);
	virtual void SetScaleZ(float z);

	bool IsRenderingReady() const { return mRenderingReady; }
	virtual void PrepareRendering(){}

	bool IsVisible() const { return mVisible; }

protected:

	virtual void CreateRenderData() {}
	virtual void CreateVertexData(){}

	SSMeshVertexData mVertexData;
	SSMeshRenderData mRenderData;	
	
	friend class SSGameObjectManager;
	DirectX::XMFLOAT3 mPosition;
	DirectX::XMFLOAT3 mScale;

	bool mVisible = true;

	float mYaw = 0;
	float mPitch = 0;
	float mRoll = 0;
	UINT mObjectId = 0;

	std::shared_ptr<class SSMaterial> mMaterial;
	
	//
	bool mRenderingReady = false;	
};