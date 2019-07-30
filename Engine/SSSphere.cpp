
#include "Core.h"
#include "SSSphere.h"



SSSphere::SSSphere(UINT sector, UINT stack, float radius)
	: mSectorCount(sector), mStackCount(stack), mRadius(radius)
{
	
}


void SSSphere::InternalCreate()
{
	for (UINT i = 0; i < mStackCount; ++i)
	{
		auto degree1 = (180.0f / (float)mStackCount) * (float)i;
		auto degree2 = (180.0f / (float)mStackCount) * (float)(i + 1);

		auto sectorRadius1 = mRadius * DirectX::XMScalarSin(XMConvertToRadians(degree1));
		auto sectorRadius2 = mRadius * DirectX::XMScalarSin(XMConvertToRadians(degree2));

		auto y1 = mRadius * DirectX::XMScalarCos(XMConvertToRadians(degree1));
		auto y2 = mRadius * DirectX::XMScalarCos(XMConvertToRadians(degree2));

		auto v1 = (float)i / (float)mStackCount;
		auto v2 = (float)(i + 1) / (float)mStackCount;

		for (UINT j = 0; j < mSectorCount; ++j)
		{
			auto deg1 = (360 / (float)mSectorCount) * j;
			auto deg2 = (360 / (float)mSectorCount) * (j + 1);

			auto u1 = (float)j / (float)mSectorCount;
			auto u2 = (float)(j + 1) / (float)mSectorCount;

			// v1
			auto x1 = sectorRadius1 * DirectX::XMScalarCos(XMConvertToRadians(deg1));
			auto z1 = sectorRadius1 * DirectX::XMScalarSin(XMConvertToRadians(deg1));

			// v2
			auto x2 = sectorRadius1 * DirectX::XMScalarCos(XMConvertToRadians(deg2));
			auto z2 = sectorRadius1 * DirectX::XMScalarSin(XMConvertToRadians(deg2));

			// v3
			auto x3 = sectorRadius2 * DirectX::XMScalarCos(XMConvertToRadians(deg1));
			auto z3 = sectorRadius2 * DirectX::XMScalarSin(XMConvertToRadians(deg1));

			// v4
			auto x4 = sectorRadius2 * DirectX::XMScalarCos(XMConvertToRadians(deg2));
			auto z4 = sectorRadius2 * DirectX::XMScalarSin(XMConvertToRadians(deg2));

			auto V1 = XMFLOAT3{ (float)x1, (float)y1, (float)z1 };
			auto T1 = XMFLOAT2{ u1, v1 };

			auto V2 = XMFLOAT3((float)x2, (float)y1, (float)z2);
			auto T2 = XMFLOAT2(u2, v1);

			auto V3 = XMFLOAT3((float)x3, (float)y2, (float)z3);
			auto T3 = XMFLOAT2(u1, v2);

			auto V4 = XMFLOAT3((float)x4, (float)y2, (float)z4);
			auto T4 = XMFLOAT2(u2, v2);

						
			XMFLOAT3 norm1; XMStoreFloat3(&norm1, XMVector3Normalize(XMLoadFloat3(&V1)));
			XMFLOAT3 norm2; XMStoreFloat3(&norm2, XMVector3Normalize(XMLoadFloat3(&V2)));
			XMFLOAT3 norm3; XMStoreFloat3(&norm3, XMVector3Normalize(XMLoadFloat3(&V3)));
			XMFLOAT3 norm4; XMStoreFloat3(&norm4, XMVector3Normalize(XMLoadFloat3(&V4)));

			// V1-----V2
			//       /
			//      /
			//     /
			//    /
			//   /
			//  /
			// V3-----V4

			mTempVertexList.push_back(V1);
			mTempVertexList.push_back(V2);
			mTempVertexList.push_back(V3);

			mTempVertexList.push_back(V3);
			mTempVertexList.push_back(V2);
			mTempVertexList.push_back(V4);

			mTempNormalList.push_back(norm1);
			mTempNormalList.push_back(norm2);
			mTempNormalList.push_back(norm3);

			mTempNormalList.push_back(norm3);
			mTempNormalList.push_back(norm2);
			mTempNormalList.push_back(norm4);

			mTempTexCoordList.push_back(T1);
			mTempTexCoordList.push_back(T2);
			mTempTexCoordList.push_back(T3);

			mTempTexCoordList.push_back(T3);
			mTempTexCoordList.push_back(T2);
			mTempTexCoordList.push_back(T4);
		}
	}
}


void SSSphere::GenerateTangents()
{
	/*List<Vector3> tan1Accum = new List<Vector3>();
	List<Vector3> tan2Accum = new List<Vector3>();

	for (uint i = 0; i < TempVertexList.Count; ++i)
	{
		tan1Accum.Add(new Vector3(0, 0, 0));
		tan2Accum.Add(new Vector3(0, 0, 0));
	}

	for (uint i = 0; i < TempVertexList.Count; i++)
	{
		TempTangentList.Add(new Vector4(0, 0, 0, 0));
	}

	// Compute the tangent vector
	for (uint i = 0; i < TempVertexList.Count; i += 3)
	{
		var p1 = TempVertexList[(int)i];
		var p2 = TempVertexList[(int)i + 1];
		var p3 = TempVertexList[(int)i + 2];

		var tc1 = TempTexCoordList[(int)i];
		var tc2 = TempTexCoordList[(int)i + 1];
		var tc3 = TempTexCoordList[(int)i + 2];

		Vector3 q1 = p2 - p1;
		Vector3 q2 = p3 - p1;
		float s1 = tc2.X - tc1.X, s2 = tc3.X - tc1.X;
		float t1 = tc2.Y - tc1.Y, t2 = tc3.Y - tc1.Y;

		// prevent degeneration
		float r = 1.0f / (s1 * t2 - s2 * t1);
		if (Single.IsInfinity(r))
		{
			r = 1 / 0.1f;
		}

		var tan1 = new Vector3((t2 * q1.X - t1 * q2.X) * r,
			(t2 * q1.Y - t1 * q2.Y) * r,
			(t2 * q1.Z - t1 * q2.Z) * r);

		var tan2 = new Vector3((s1 * q2.X - s2 * q1.X) * r,
			(s1 * q2.Y - s2 * q1.Y) * r,
			(s1 * q2.Z - s2 * q1.Z) * r);


		tan1Accum[(int)i] += tan1;
		tan1Accum[(int)i + 1] += tan1;
		tan1Accum[(int)i + 2] += tan1;

		tan2Accum[(int)i] += tan2;
		tan2Accum[(int)i + 1] += tan2;
		tan2Accum[(int)i + 2] += tan2;
	}

	Vector4 lastValidTangent = new Vector4();

	for (uint i = 0; i < TempVertexList.Count; ++i)
	{
		var n = TempNormalList[(int)i];
		var t1 = tan1Accum[(int)i];
		var t2 = tan2Accum[(int)i];

		// Gram-Schmidt orthogonalize                
		var temp = OpenTK.Vector3.Normalize(t1 - (OpenTK.Vector3.Dot(n, t1) * n));
		// Store handedness in w                
		var W = (OpenTK.Vector3.Dot(OpenTK.Vector3.Cross(n, t1), t2) < 0.0f) ? -1.0f : 1.0f;

		bool bValid = true;
		if (Single.IsNaN(temp.X) || Single.IsNaN(temp.Y) || Single.IsNaN(temp.Z))
		{
			bValid = false;
		}

		if (Single.IsInfinity(temp.X) || Single.IsInfinity(temp.Y) || Single.IsInfinity(temp.Z))
		{
			bValid = false;
		}

		if (bValid == true)
		{
			lastValidTangent = new Vector4(temp.X, temp.Y, temp.Z, W);
		}

		if (bValid == false)
		{
			temp = lastValidTangent.Xyz;
		}

		TempTangentList[(int)i] = new Vector4(temp.X, temp.Y, temp.Z, W);
	}

	tan1Accum.Clear();
	tan2Accum.Clear();
	*/
}




bool SSSphere::bIsInitialized = false;
SSVertexBuffer* SSSphere::mSphereVB = nullptr;
SSIndexBuffer* SSSphere::mSphereIB = nullptr;