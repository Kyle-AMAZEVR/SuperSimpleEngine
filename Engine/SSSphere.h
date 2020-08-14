
#pragma once

#include "SSGameObject.h"
#include "SSName.h"
#include <vector>

class ENGINE_API SSSphere : public SSGameObject
{
public:
	SSSphere(UINT sector, UINT stack, float radius);

	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material);
	virtual void DebugDraw(ID3D11DeviceContext* deviceConttext, class SSMaterial* material);

	void SetMetalicValue(float metal) { mMetalic = metal; }
	void SetRoughnessValue(float rough) { mRoughness = rough; }


protected:
	UINT mSectorCount = 5;
	UINT mStackCount = 5;
	float mRadius = 5.0f;


	void InternalCreate();
	void GenerateTangents();

	static bool bIsInitialized;
	static class SSDX11VertexBuffer* mSphereVB;

	std::shared_ptr<SSDX11VertexBuffer> mDebugTBNVB;
	std::shared_ptr<class SSIndexBuffer> mDebugIB;

	float mRoughness = 0.1f;
	float mMetalic = 0.9f;


	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;	

	bool bCreateDebugTBN = false;
};


class ENGINE_API SSPBRSphere : public SSSphere
{
public:
	SSPBRSphere(ID3D11DeviceContext* deviceContext, SSName diffuseTex, SSName normalTex, SSName metalTex, SSName roughTex);

	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material) override;
protected:

	std::shared_ptr<class SSTexture2D> mDiffuseTex;
	std::shared_ptr<class SSTexture2D> mNormalTex;
	std::shared_ptr<class SSTexture2D> mMetalTex;
	std::shared_ptr<class SSTexture2D> mRoughTex;

	SSName mDiffuseTexName;
	SSName mNormalTexName;
	SSName mMetalTexName;
	SSName mRoughTexName;


};