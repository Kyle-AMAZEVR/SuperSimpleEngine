#include "Core.h"
#include "SSObjMesh.h"
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "DXVertexBuffer.h"
#include "SSISerializable.h"
#include "SSObjMeshMaterial.h"
#include "SSMaterial.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"
#include "SSFreqUsedNames.h"
#include "CameraManager.h"
#include "SSIndexBuffer.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSMathHelper.h"
#include "SSSamplerManager.h"

// trim from start (in place)
static inline void ltrim(std::string &s) 
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) 
	{
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string &s) 
{
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) 
	{
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string &s) 
{
	ltrim(s);
	rtrim(s);
}

// trim from start (copying)
static inline std::string ltrim_copy(std::string s) 
{
	ltrim(s);
	return s;
}

// trim from end (copying)
static inline std::string rtrim_copy(std::string s) 
{
	rtrim(s);
	return s;
}

// trim from both ends (copying)
static inline std::string trim_copy(std::string s) 
{
	trim(s);
	return s;
}


bool SSObjMesh::ImportObjFile(const std::string& FilePath, const std::string& MtlFilePath)
{
	ParseMtlFile(MtlFilePath);

	std::ifstream in(FilePath.c_str(), std::ios::in);
	// @import file
	if (in.good())
	{
		std::string Line;
		bool bMtlStart = false;
		unsigned int PreviousMtlVertexIndex = 0;

		while (std::getline(in, Line))
		{
			if (Line.size() <= 1)
			{
				continue;
			}

			if (Line[0] == 'v' && std::isspace(Line[1]))
			{
				//@ Vertex
				mTempVertexList.push_back(SSObjMeshParser::ParseVertex(Line));
			}
			else if (Line[0] == 'v' && Line[1] == 't')
			{
				//@ Texcoord
				mTempTexCoordList.push_back(std::move(SSObjMeshParser::ParseTexcoord(Line)));
				mHasTexCoord = true;
			}
			else if (Line[0] == 'v' && Line[1] == 'n')
			{
				//@ Normal
				mTempNormalList.push_back(std::move(SSObjMeshParser::ParseNormal(Line)));
				mHasNormal = true;
			}
			else if (Line[0] == 'f' && std::isspace(Line[1]))
			{
				//@Face Info
				SSObjMeshParser::ParseFace(Line, mVertexIndexList, mTexcoordIndexList, mNormalIndexList);
			}
			else if(Line[0] == 'u' && Line[1] == 's' && Line[2] == 'e')
			{
				char buffer[256]{'\0'};

				sscanf(Line.c_str(), "usemtl %s", buffer);

				if(mMeshSectionList.size() == 0)
				{
					SSObjMeshSection newSection{ buffer, 0 };
					mMeshSectionList.push_back(newSection);
				}				
				else
				{
					if (mMeshSectionList[mMeshSectionList.size() - 1].mSectionName != buffer)
					{
						mMeshSectionList[mMeshSectionList.size() - 1].SetEndIndex(static_cast<UINT>(mVertexIndexList.size()));

						SSObjMeshSection newSection{ buffer, static_cast<UINT>(mVertexIndexList.size()) };

						mMeshSectionList.push_back(newSection);
					}
				}
			}
		}
	}

	if (mMeshSectionList.size() > 0)
	{
		if (mMeshSectionList[mMeshSectionList.size() - 1].mEndIndex == 0)
		{
			mMeshSectionList[mMeshSectionList.size() - 1].SetEndIndex(static_cast<UINT>(mVertexIndexList.size()));
		}
	}

	// @parsing done
	// @create Tangents
	if (mHasTexCoord)
	{
		GenerateTangents();
	}

	check(mVertexIndexList.size() == mNormalIndexList.size());
	check(mVertexIndexList.size() == mTexcoordIndexList.size());
	check(mVertexIndexList.size() == mTempTangentList.size());

	// @create real vertex
	mRealVertexList.resize(mVertexIndexList.size());
	
	for(UINT i = 0; i < mVertexIndexList.size(); ++i)
	{
		VT_PositionNormalTexcoordTangent v
		{
			mTempVertexList[mVertexIndexList[i]],
			mTempNormalList[mNormalIndexList[i]],
			mTempTexCoordList[mTexcoordIndexList[i]],
			mTempTangentList[i],
		};
		mRealVertexList[i] = std::move(v);
	}

	// @clear 
	mTempVertexList.clear();
	mTempTangentList.clear();
	mTempTexCoordList.clear();
	mTempNormalList.clear();

	SerializeWriter writer("./Prebaked/sponza.mesh");
	writer << mRealVertexList;
	writer << mMeshSectionList;

	writer << static_cast<UINT>(mMeshMaterialMap.size());

	for(auto& kvp : mMeshMaterialMap)
	{
		writer << kvp.first;
		writer << kvp.second;		
	}
	//writer << mMeshMaterialMap;

	mVB = std::make_shared<SSVertexBuffer>();
	mVB->SetVertexBufferData(mRealVertexList);

	std::vector<UINT> idx;
	idx.resize(mRealVertexList.size());

	for (UINT i = 0; i < mRealVertexList.size(); ++i)
	{
		idx[i] = i;
	}

	mIB = std::make_shared<SSIndexBuffer>();
	mIB->SetIndexBufferData(idx);
	
	return true;
}

bool SSObjMesh::LoadCookedFile(const std::string& filePath)
{
	SerializeReader reader(filePath);
	if(reader.IsGood())
	{
		UINT materialMapSize = 0;

		reader >> mRealVertexList;
		reader >> mMeshSectionList;
		reader >> materialMapSize;

		for(UINT i = 0; i < materialMapSize; ++i)
		{
			std::string name;
			SSObjMeshMaterial material;

			reader >> name;
			reader >> material;

			mMeshMaterialMap[name] = material;
		}
		
		mVB = std::make_shared<SSVertexBuffer>();
		mVB->SetVertexBufferData(mRealVertexList);

		std::vector<UINT> idx;
		idx.resize(mRealVertexList.size());

		for (UINT i = 0; i < mRealVertexList.size(); ++i)
		{
			idx[i] = i;
		}

		mIB = std::make_shared<SSIndexBuffer>();
		mIB->SetIndexBufferData(idx);

		return true;
	}
	else
	{
		return false;
	}
}


bool operator< (const VT_PositionNormalTexcoordTangent& a, const VT_PositionNormalTexcoordTangent& b)
{
	if(a.VertexAttribute1.x < a.VertexAttribute1.x)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void SSObjMesh::Draw(ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	auto stride = mVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVB->GetBufferPointerRef(), &stride, &offset);	

	deviceContext->Draw(mVB->GetVertexCount(), 0);	
}

void SSObjMesh::Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material)
{
	check(material != nullptr);

	material->SetCurrent();

	//material->SetVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixScaling(0.3,0.3,0.3) * XMMatrixTranslation(0, -20, 0) ));
	material->SetVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0) * XMMatrixTranslation(10, -100, 0)));
	material->SetVSConstantBufferData(ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	material->SetVSConstantBufferData(ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 0; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();

	material->SetPSConstantBufferData("TextureExist", settings);	
	material->SetPSSampler("DefaultTexSampler", sampler);

	auto stride = mVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, &mVB->GetBufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mIB->GetBufferPointer(), DXGI_FORMAT::DXGI_FORMAT_R32_UINT, 0);

	for(auto& section : mMeshSectionList)
	{		
		if (mMeshMaterialMap.count(section.mSectionName) > 0)
		{
			auto diffuse = SSTextureManager::Get().LoadTexture2D(mMeshMaterialMap[section.mSectionName].mDiffuseMap);

			material->SetPSTexture("DiffuseTex", diffuse.get());

			deviceContext->DrawIndexed(section.mEndIndex - section.mStartIndex, section.mStartIndex, 0);
		}		
	}
}

void SSObjMesh::OptimizedGenerateVertices()
{	
	
}

bool SSObjMesh::GetSimilarVertexIndex(VT_PositionNormalTexcoordTangent& vertex, UINT& index)
{
	UINT vHashCode = GetHashCode(vertex);

	if (mVertexCacheMap.count(vHashCode) > 0)
	{
		
		return true;
	}
	else
	{
		index = 0;
		return false;
	}
}

bool SSObjMesh::ParseMtlFile(const std::string& filepath)
{
	std::ifstream in(filepath.c_str(), std::ios::in);
	// @import file
	if (in.good())
	{
		std::string Line;
		bool newMtlStarted = false;

		SSObjMeshMaterial* newMaterial = nullptr;

		while (std::getline(in, Line))
		{
			trim(Line);

			//								

			if (Line.size() > 0)
			{
				char buffer[256] {0};

				if (sscanf(Line.c_str(), "newmtl %s", buffer) > 0)
				{
					newMaterial = new SSObjMeshMaterial();
					newMaterial->mMaterialName = buffer;
					newMtlStarted = true;
				}
				else if (sscanf(Line.c_str(), "map_Ka %s", buffer) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mMetalicMap = buffer;
				}
				else if (sscanf(Line.c_str(), "map_Kd %s", buffer) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mDiffuseMap = buffer;
				}
				else if (sscanf(Line.c_str(), "bump %s", buffer) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mNormalMap = buffer;
				}
				else if (sscanf(Line.c_str(), "map_Ns %s", buffer) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mRoughnessMap = buffer;
				}
				else if(sscanf(Line.c_str(), "map_d %s", buffer) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mMaskMap = buffer;
				}
			}
			else
			{
				if (newMtlStarted)
				{
					check(newMaterial != nullptr);
					mMeshMaterialMap[newMaterial->mMaterialName] = *newMaterial;
					delete newMaterial;
					newMaterial = nullptr;
					newMtlStarted = false;
				}
			}
		}

		if (newMtlStarted == true && newMaterial != nullptr)
		{
			mMeshMaterialMap[newMaterial->mMaterialName] = *newMaterial;
			delete newMaterial;
			newMaterial = nullptr;
		}
	}

	return true;
}


void SSObjMesh::GenerateTangents()
{
	std::vector<XMVECTOR> tan1Accum;
	std::vector<XMVECTOR> tan2Accum;

	tan1Accum.resize(mTempVertexList.size());
	tan2Accum.resize(mTempVertexList.size());

	mTempTangentList.resize(mVertexIndexList.size());
	
	// Compute the tangent vector
	for (UINT i = 0; i < mVertexIndexList.size(); i += 3)
	{
		XMVECTOR p1 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i]]);
		XMVECTOR p2 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i+1]]);
		XMVECTOR p3 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i+2]]);

		XMVECTOR tc1 = XMLoadFloat2(&mTempTexCoordList[mTexcoordIndexList[i]]);
		XMVECTOR tc2 = XMLoadFloat2(&mTempTexCoordList[mTexcoordIndexList[i+1]]);
		XMVECTOR tc3 = XMLoadFloat2(&mTempTexCoordList[mTexcoordIndexList[i+2]]);

		XMFLOAT3 q1; XMStoreFloat3(&q1, p2 - p1);
		XMFLOAT3 q2; XMStoreFloat3(&q2, p3 - p1);

		float s1 = mTempTexCoordList[mTexcoordIndexList[i+1]].x - mTempTexCoordList[mTexcoordIndexList[i]].x;
		float s2 = mTempTexCoordList[mTexcoordIndexList[i+2]].x - mTempTexCoordList[mTexcoordIndexList[i]].x;

		float t1 = mTempTexCoordList[mTexcoordIndexList[i+1]].y - mTempTexCoordList[mTexcoordIndexList[i]].y;
		float t2 = mTempTexCoordList[mTexcoordIndexList[i+2]].y - mTempTexCoordList[mTexcoordIndexList[i]].y;

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

		tan1Accum[mVertexIndexList[i]] += tan1;
		tan1Accum[mVertexIndexList[i+1]] += tan1;
		tan1Accum[mVertexIndexList[i+2]] += tan1;

		tan2Accum[mVertexIndexList[i]] += tan2;
		tan2Accum[mVertexIndexList[i+1]] += tan2;
		tan2Accum[mVertexIndexList[i+2]] += tan2;
	}

	XMFLOAT4 lastValidTangent;

	for (UINT i = 0; i < mVertexIndexList.size(); ++i)
	{
		auto n1 = mNormalIndexList[i];
		if (n1 >= mTexcoordIndexList.size())
		{
			check(n1 < mTempNormalList.size());
		}

		auto n = DirectX::XMLoadFloat3(&mTempNormalList[n1]);

		auto i1 = mVertexIndexList[i];
		check(i1 < tan1Accum.size());
		auto t1 = tan1Accum[i1];
		auto t2 = tan2Accum[i1];

		// Gram-Schmidt orthogonalize                
		XMFLOAT3 temp;
		XMStoreFloat3(&temp, XMVector3Normalize(t1 - (XMVector3Dot(n, t1) * n)));
		// Store handedness in w                
		XMFLOAT3 dotResult;
		XMStoreFloat3(&dotResult, XMVector3Dot(XMVector3Cross(n, t1), t2));

		auto W = (dotResult.x < 0.0f) ? -1.0f : 1.0f;

		bool bValid = true;
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

}



void SSObjMeshParser::ParseFace(std::string& line, std::vector<UINT>& vertexIndexList, std::vector<UINT>& texcoordIndexList, std::vector<UINT>& normalIndexList)
{
	UINT v1, v2, v3;
	UINT n1, n2, n3;
	UINT t1, t2, t3;	

	int count = sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", 
		&v1, &t1, &n1,
		&v2, &t2, &n2,
		&v3, &t3, &n3);

	vertexIndexList.push_back(v1-1); vertexIndexList.push_back(v2-1); vertexIndexList.push_back(v3-1);

	texcoordIndexList.push_back(t1-1); texcoordIndexList.push_back(t2-1); texcoordIndexList.push_back(t3-1);

	normalIndexList.push_back(n1-1); normalIndexList.push_back(n2-1); normalIndexList.push_back(n3-1);
}





XMFLOAT4 SSObjMeshParser::ParseVertex(std::string& line)
{
	XMFLOAT4 result;

	sscanf_s(line.c_str(), "v %f %f %f", &result.x, &result.y, &result.z);
	
	// for opengl resource revert z	
	// result.z = -result.z;

	result.w = 1.0f;

	return result;
}

XMFLOAT3 SSObjMeshParser::ParseNormal(std::string& line)
{
	XMFLOAT3 result;

	sscanf_s(line.c_str(), "vn %f %f %f", &result.x, &result.y, &result.z);	

	return result;
}

 XMFLOAT2 SSObjMeshParser::ParseTexcoord(std::string& line)
{
	 XMFLOAT2 result;

	 sscanf_s(line.c_str(), "vt %f %f", &result.x, &result.y);

	 // for opengl resource
	 result.y = 1 - result.y;

	 return result;

}