
#include "SSCommon.h"
#include "SSTangentVectorGenerator.h"

std::vector<XMFLOAT4> SSTangentVectorGenerator::GenerateTangents(const std::vector<XMFLOAT4>& tempVertexList,
	const std::vector<XMFLOAT3>& tempNormalList,
	const std::vector<XMFLOAT2>& tempTexCoordList)
{
	std::vector<XMFLOAT4> tempTangentList;

	std::vector<XMVECTOR> tan1Accum;
	std::vector<XMVECTOR> tan2Accum;

	for (UINT i = 0; i < tempVertexList.size(); ++i)
	{
		tan1Accum.push_back(XMVECTOR());
		tan2Accum.push_back(XMVECTOR());
	}

	for (UINT i = 0; i < tempVertexList.size(); i++)
	{
		tempTangentList.push_back(XMFLOAT4(0, 0, 0, 0));
	}

	// Compute the tangent vector
	for (UINT i = 0; i < tempVertexList.size(); i += 3)
	{
		XMVECTOR p1 = XMLoadFloat4(&tempVertexList[(int)i]);
		XMVECTOR p2 = XMLoadFloat4(&tempVertexList[(int)i + 1]);
		XMVECTOR p3 = XMLoadFloat4(&tempVertexList[(int)i + 2]);

		XMVECTOR tc1 = XMLoadFloat2(&tempTexCoordList[(int)i]);
		XMVECTOR tc2 = XMLoadFloat2(&tempTexCoordList[(int)i + 1]);
		XMVECTOR tc3 = XMLoadFloat2(&tempTexCoordList[(int)i + 2]);

		XMFLOAT3 q1; XMStoreFloat3(&q1, p2 - p1);
		XMFLOAT3 q2; XMStoreFloat3(&q2, p3 - p1);

		float s1 = tempTexCoordList[(int)i + 1].x - tempTexCoordList[(int)i].x;
		float s2 = tempTexCoordList[(int)i + 2].x - tempTexCoordList[(int)i].x;

		float t1 = tempTexCoordList[(int)i + 1].y - tempTexCoordList[(int)i].y;
		float t2 = tempTexCoordList[(int)i + 2].y - tempTexCoordList[(int)i].y;

		// prevent degeneration
		float r = 1.0f / (s1 * t2 - s2 * t1);

		if (std::isinf(r))
		{
			r = 1 / 0.1f;
		}

		XMFLOAT3 tmptan1;
		tmptan1.x = (t2 * q1.x - t1 * q2.x) * r;
		tmptan1.y = (t2 * q1.y - t1 * q2.y) * r;
		tmptan1.z = (t2 * q1.z - t1 * q2.z) * r;

		XMFLOAT3 tmptan2;
		tmptan2.x = (s1 * q2.x - s2 * q1.x) * r;
		tmptan2.y = (s1 * q2.y - s2 * q1.y) * r;
		tmptan2.z = (s1 * q2.z - s2 * q1.z) * r;

		auto tan1 = XMLoadFloat3(&tmptan1);
		auto tan2 = XMLoadFloat3(&tmptan2);

		tan1Accum[(int)i] += tan1;
		tan1Accum[(int)i + 1] += tan1;
		tan1Accum[(int)i + 2] += tan1;

		tan2Accum[(int)i] += tan2;
		tan2Accum[(int)i + 1] += tan2;
		tan2Accum[(int)i + 2] += tan2;
	}

	XMFLOAT4 lastValidTangent(0, 0, 0, 0);

	for (UINT i = 0; i < tempVertexList.size(); ++i)
	{
		auto n = DirectX::XMLoadFloat3(&tempNormalList[(int)i]);
		auto t1 = tan1Accum[(int)i];
		auto t2 = tan2Accum[(int)i];

		// Gram-Schmidt orthogonalize                
		XMFLOAT3 temp;
		//XMStoreFloat3(&temp, XMVector3Normalize(t1 - (XMVector3Dot(n, t1) * n)));

		XMFLOAT3 normalDotTangent;
		XMStoreFloat3(&normalDotTangent, XMVector3Dot(n, t1));

		XMVECTOR computeResult = t1 - XMVectorScale(n, normalDotTangent.x);
		XMFLOAT4 errCheck;
		XMStoreFloat4(&errCheck, computeResult);

		bool bValid = true;

		if (errCheck.x == 0 && errCheck.y == 0 && errCheck.z == 0 && errCheck.w == 0)
		{
			bValid = false;
		}

		XMStoreFloat3(&temp, XMVector3Normalize(computeResult));

		// Store handedness in w                
		XMFLOAT3 dotResult;
		XMStoreFloat3(&dotResult, XMVector3Dot(XMVector3Cross(n, t1), t2));

		auto W = (dotResult.x < 0.0f) ? -1.0f : 1.0f;

		if (std::isnan(temp.x) || std::isnan(temp.y) || std::isnan(temp.z))
		{
			bValid = false;
		}

		if (std::isinf(temp.x) || std::isinf(temp.y) || std::isinf(temp.z))
		{
			bValid = false;
		}

		if (bValid == true)
		{
			lastValidTangent.x = temp.x;
			lastValidTangent.y = temp.y;
			lastValidTangent.z = temp.z;
			lastValidTangent.w = W;
		}

		if (bValid == false)
		{
			temp.x = lastValidTangent.x;
			temp.y = lastValidTangent.y;
			temp.z = lastValidTangent.z;
		}

		tempTangentList[i].x = temp.x;
		tempTangentList[i].y = temp.y;
		tempTangentList[i].z = temp.z;
		tempTangentList[i].w = W;
	}

	tan1Accum.clear();

	return std::move(tempTangentList);
}
