
#pragma once

#include "SSSceneObject.h"
#include <vector>

class ENGINE_API SSSphere : public SSSceneObject
{
public:
	SSSphere(UINT sector, UINT stack, float radius);

	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material);
	virtual void DebugDraw(ID3D11DeviceContext* deviceConttext, class SSMaterial* material);
protected:
	UINT mSectorCount = 5;
	UINT mStackCount = 5;
	float mRadius = 5.0f;


	void InternalCreate();
	void GenerateTangents();

	static bool bIsInitialized;
	static class SSVertexBuffer* mSphereVB;

	std::shared_ptr<SSVertexBuffer> mDebugTBNVB;

	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;
};
