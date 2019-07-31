
#pragma once

#include "SSSceneObject.h"
#include <vector>

class ENGINE_API SSSphere : public SSSceneObject
{
public:
	SSSphere(UINT sector, UINT stack, float radius);

protected:
	UINT mSectorCount = 5;
	UINT mStackCount = 5;
	float mRadius = 5.0f;


	void InternalCreate();
	void GenerateTangents();

	static bool bIsInitialized;
	static class SSVertexBuffer* mSphereVB;
	static class SSIndexBuffer* mSphereIB;

	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;
};
