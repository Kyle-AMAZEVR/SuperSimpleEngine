
#pragma once

#include "SSGameObject.h"
#include "SSName.h"
#include <vector>
#include "SSVertexTypes.h"

class GAMEMODULE_API SSSphere : public SSGameObject
{
public:
	SSSphere(UINT sector, UINT stack, float radius);

	virtual void OnAddedScene() override;
	
	/*virtual void Draw(SSDX11Renderer* renderer) override;
	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material);
	virtual void DebugDraw(ID3D11DeviceContext* deviceConttext, class SSMaterial* material);*/

	void SetMetalicValue(float metal) { mMetalic = metal; }
	void SetRoughnessValue(float rough) { mRoughness = rough; }


protected:
	UINT mSectorCount = 5;
	UINT mStackCount = 5;
	float mRadius = 5.0f;

	void InternalCreate();
	void GenerateTangents();

	static bool bIsInitialized;		

	float mRoughness = 0.1f;
	float mMetalic = 0.9f;

	// temp 
	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;
	
	static std::vector< VT_PositionNormalTexcoordTangent > mVertexArray;

	bool bCreateDebugTBN = false;
};


class ENGINE_API SSPBRSphere : public SSSphere
{
public:
	SSPBRSphere(ID3D11DeviceContext* deviceContext, SSName diffuseTex, SSName normalTex, SSName metalTex, SSName roughTex);

	// virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material) override;
protected:

	SSName mDiffuseTexName;
	SSName mNormalTexName;
	SSName mMetalTexName;
	SSName mRoughTexName;


};