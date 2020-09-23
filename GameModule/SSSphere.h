
#pragma once

#include "SSGameObject.h"
#include "SSName.h"
#include <vector>
#include "SSVertexTypes.h"

class GAMEMODULE_API SSSphere : public SSGameObject
{
public:
	SSSphere(UINT sector, UINT stack, float radius);
	void SetMetalicValue(float metal) { mMetalic = metal; }
	void SetRoughnessValue(float rough) { mRoughness = rough; }

    virtual void Tick(float delta) override;
protected:
	UINT mSectorCount = 5;
	UINT mStackCount = 5;
	float mRadius = 5.0f;

	void CreateVertexData() override;
	void CreateRenderData() override;

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


class GAMEMODULE_API SSPBRSphere : public SSSphere
{
public:
    SSPBRSphere();
	SSPBRSphere(SSName diffuseTex, SSName normalTex, SSName metalTex, SSName roughTex);
	virtual void CreateRenderData() override;

protected:
	SSName mDiffuseTexName;
	SSName mNormalTexName;
	SSName mMetalTexName;
	SSName mRoughTexName;
};