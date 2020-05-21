
#include "Core.h"
#include "SSSphere.h"
#include <cmath>
#include "DXVertexTypes.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSMaterial.h"
#include "SSFreqUsedNames.h"
#include "SSName.h"
#include "SSCameraManager.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSSamplerManager.h"
#include "SSTextureManager.h"
#include "SSMathHelper.h"
#include "SSTexture2D.h"

SSSphere::SSSphere(UINT sector, UINT stack, float radius)
	: mSectorCount(sector), mStackCount(stack), mRadius(radius)
{
	if (bIsInitialized == false)
	{
		InternalCreate();
		bIsInitialized = true;
	}
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

			auto V1 = XMFLOAT4{ (float)x1, (float)y1, (float)z1 , 1};
			auto T1 = XMFLOAT2(u1, v1 );

			auto V2 = XMFLOAT4((float)x2, (float)y1, (float)z2, 1);
			auto T2 = XMFLOAT2(u2, v1);

			auto V3 = XMFLOAT4((float)x3, (float)y2, (float)z3, 1);
			auto T3 = XMFLOAT2(u1, v2);

			auto V4 = XMFLOAT4((float)x4, (float)y2, (float)z4, 1);
			auto T4 = XMFLOAT2(u2, v2);


			XMFLOAT3 norm1; XMStoreFloat3(&norm1, XMVector3Normalize(XMLoadFloat4(&V1)));
			XMFLOAT3 norm2; XMStoreFloat3(&norm2, XMVector3Normalize(XMLoadFloat4(&V2)));
			XMFLOAT3 norm3; XMStoreFloat3(&norm3, XMVector3Normalize(XMLoadFloat4(&V3)));
			XMFLOAT3 norm4; XMStoreFloat3(&norm4, XMVector3Normalize(XMLoadFloat4(&V4)));

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

	GenerateTangents();

	std::vector< VT_PositionNormalTexcoordTangent > vertexArray;
	std::vector<UINT> indexArray;
	
	for (UINT i = 0; i < mTempVertexList.size(); i += 3)
	{
		// face1
		vertexArray.push_back(VT_PositionNormalTexcoordTangent
		(
			mTempVertexList[i], mTempNormalList[i], mTempTexCoordList[i], mTempTangentList[i]
		));

		vertexArray.push_back(VT_PositionNormalTexcoordTangent
		(
			mTempVertexList[i + 1], mTempNormalList[i + 1], mTempTexCoordList[i + 1], mTempTangentList[i + 1]
		));

		vertexArray.push_back(VT_PositionNormalTexcoordTangent
		(
			mTempVertexList[i + 2], mTempNormalList[i + 2], mTempTexCoordList[i + 2], mTempTangentList[i + 2]
		));		
	}

	// debug purpose
	if (bCreateDebugTBN)
	{
		std::vector<VT_PositionColor> tbnVertexArray;
		for (UINT i = 0; i < mTempNormalList.size(); i++)
		{
			// 
			tbnVertexArray.push_back(VT_PositionColor
			(
				mTempVertexList[i], SSMathHelper::UnitY3
			));

			indexArray.push_back(static_cast<UINT>(tbnVertexArray.size() - 1));

			XMFLOAT4 normalEnd;
			XMStoreFloat4(&normalEnd, XMLoadFloat4(&mTempVertexList[i]) + XMVectorScale(XMLoadFloat3(&mTempNormalList[i]), 1.0f));

			tbnVertexArray.push_back(VT_PositionColor
			(
				normalEnd, SSMathHelper::UnitY3
			));

			indexArray.push_back(static_cast<UINT>(tbnVertexArray.size() - 1));

			XMFLOAT4 tangentEnd;

			XMFLOAT3 temp(mTempTangentList[i].x, mTempTangentList[i].y, mTempTangentList[i].z);

			XMStoreFloat4(&tangentEnd, XMLoadFloat4(&mTempVertexList[i]) + XMVectorScale(XMLoadFloat3(&temp), 1.0f));

			tbnVertexArray.push_back(VT_PositionColor
			(
				mTempVertexList[i], SSMathHelper::UnitZ3
			));

			indexArray.push_back(static_cast<UINT>(tbnVertexArray.size() - 1));

			tbnVertexArray.push_back(VT_PositionColor
			(
				tangentEnd, SSMathHelper::UnitZ3
			));

			indexArray.push_back(static_cast<UINT>(tbnVertexArray.size() - 1));
		}

		mDebugTBNVB = std::make_shared<SSDX11VertexBuffer>();
		mDebugTBNVB->SetVertexBufferData(tbnVertexArray);

		mDebugIB = std::make_shared<SSIndexBuffer>();
		mDebugIB->SetIndexBufferData(indexArray);
	}


	mSphereVB = new SSDX11VertexBuffer();
	mSphereVB->SetVertexBufferData(vertexArray);	
}


void SSSphere::GenerateTangents()
{
	std::vector<XMVECTOR> tan1Accum;
	std::vector<XMVECTOR> tan2Accum;	

	for (UINT i = 0; i < mTempVertexList.size(); ++i)
	{
		tan1Accum.push_back(XMVECTOR());
		tan2Accum.push_back(XMVECTOR());
	}

	for (UINT i = 0; i < mTempVertexList.size(); i++)
	{
		mTempTangentList.push_back(XMFLOAT4(0, 0, 0, 0));
	}

	// Compute the tangent vector
	for (UINT i = 0; i < mTempVertexList.size(); i += 3)
	{
		XMVECTOR p1 = XMLoadFloat4(&mTempVertexList[(int)i]);
		XMVECTOR p2 = XMLoadFloat4(&mTempVertexList[(int)i + 1]);
		XMVECTOR p3 = XMLoadFloat4(&mTempVertexList[(int)i + 2]);

		XMVECTOR tc1 = XMLoadFloat2(&mTempTexCoordList[(int)i]);
		XMVECTOR tc2 = XMLoadFloat2(&mTempTexCoordList[(int)i + 1]);
		XMVECTOR tc3 = XMLoadFloat2(&mTempTexCoordList[(int)i + 2]);

		XMFLOAT3 q1; XMStoreFloat3(&q1, p2 - p1);
		XMFLOAT3 q2; XMStoreFloat3(&q2, p3 - p1);		

		float s1 = mTempTexCoordList[(int)i + 1].x - mTempTexCoordList[(int)i].x;
		float s2 = mTempTexCoordList[(int)i + 2].x - mTempTexCoordList[(int)i].x;

		float t1 = mTempTexCoordList[(int)i+1].y - mTempTexCoordList[(int)i].y;
		float t2 = mTempTexCoordList[(int)i+2].y - mTempTexCoordList[(int)i].y;

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

	XMFLOAT4 lastValidTangent(0,0,0,0);

	for (UINT i = 0; i < mTempVertexList.size(); ++i)
	{
		auto n =  DirectX::XMLoadFloat3(&mTempNormalList[(int)i]);
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

		if(errCheck.x == 0 && errCheck.y == 0 && errCheck.z ==0 && errCheck.w == 0)
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

		mTempTangentList[i].x = temp.x;
		mTempTangentList[i].y = temp.y;
		mTempTangentList[i].z = temp.z;
		mTempTangentList[i].w = W;
	}

	tan1Accum.clear();
	
}

void SSSphere::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mSphereVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mSphereVB->GetBufferPointerRef(), &stride, &offset);

	deviceContext->Draw(mSphereVB->GetVertexCount(), 0);
}

void SSSphere::DebugDraw(ID3D11DeviceContext* deviceContext, class SSMaterial* material)
{
	if (bCreateDebugTBN)
	{
		material->SetPrimitiveType(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

		auto stride = mDebugTBNVB->GetStride();
		UINT offset = 0;

		deviceContext->IASetVertexBuffers(0, 1, mDebugTBNVB->GetBufferPointerRef(), &stride, &offset);
		deviceContext->IASetIndexBuffer(mDebugIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

		material->SetCurrent();

		material->SetVSConstantBufferData(deviceContext, ModelName, XMMatrixTranspose(GetModelTransform()));
		material->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
		material->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));
		
		deviceContext->Draw(mDebugTBNVB->GetVertexCount(), 0);
	}
}

void SSSphere::Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material)
{
	check(material != nullptr);

	material->SetCurrent();

	material->SetVSConstantBufferData(deviceContext, ModelName, XMMatrixTranspose(GetModelTransform()));
	material->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	material->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 0; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 0; //normal
	settings.value4 = 0; // roghness
	settings.value5 = 0; // diffuse

	SSAlignedCBuffer<float, float> metalicRoughnessOverride;
	metalicRoughnessOverride.value1 = mMetalic;
	metalicRoughnessOverride.value2 = mRoughness;

	SSAlignedCBuffer<XMFLOAT3> DiffuseColor;
	DiffuseColor.value1.x = DiffuseColor.value1.y = DiffuseColor.value1.z = 1;

	ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();

	material->SetPSSampler("DefaultTexSampler", sampler);

	auto stride = mSphereVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mSphereVB->GetBufferPointerRef(), &stride, &offset);

	material->SetPSConstantBufferData(deviceContext, "TextureExist", settings);

	material->SetPSConstantBufferData(deviceContext, "MetalicRoughness", metalicRoughnessOverride);

	material->SetPSConstantBufferData(deviceContext, "CBDiffuseColor", DiffuseColor);

	deviceContext->Draw(mSphereVB->GetVertexCount(), 0);	
}

bool SSSphere::bIsInitialized = false;
SSDX11VertexBuffer* SSSphere::mSphereVB = nullptr;

SSPBRSphere::SSPBRSphere(ID3D11DeviceContext* deviceContext, SSName diffuseTexName, SSName normalTexName, SSName metalTexName, SSName roughTexName)
	: SSSphere(25, 25, 10.f)
{
	mMetalTex = SSTextureManager::Get().LoadTexture2D(deviceContext, metalTexName);
	mNormalTex = SSTextureManager::Get().LoadTexture2D(deviceContext, normalTexName);

	mDiffuseTex = SSTextureManager::Get().LoadTexture2D(deviceContext, diffuseTexName);
	mRoughTex = SSTextureManager::Get().LoadTexture2D(deviceContext, roughTexName);
}

void SSPBRSphere::Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material)
{
	check(material != nullptr);

	material->SetCurrent();

	material->SetVSConstantBufferData(deviceContext, ModelName,XMMatrixTranspose(GetModelTransform()));
	material->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	material->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();

	material->SetPSSampler("DefaultTexSampler", sampler);

	auto stride = mSphereVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mSphereVB->GetBufferPointerRef(), &stride, &offset);

	material->SetPSConstantBufferData(deviceContext, "TextureExist", settings);

	if (mMetalTex != nullptr)
	{
		material->SetPSTexture(deviceContext, "MetalicTex", mMetalTex.get());
	}

	if (mNormalTex != nullptr)
	{
		material->SetPSTexture(deviceContext, "NormalTex", mNormalTex.get());
	}

	if (mDiffuseTex != nullptr)
	{
		material->SetPSTexture(deviceContext, "DiffuseTex", mDiffuseTex.get());
	}

	if (mRoughTex != nullptr)
	{
		material->SetPSTexture(deviceContext, "RoughnessTex", mRoughTex.get());
	}

	deviceContext->Draw(mSphereVB->GetVertexCount(), 0);
}