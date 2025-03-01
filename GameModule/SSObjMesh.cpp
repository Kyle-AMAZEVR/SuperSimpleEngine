
#include "SSObjMesh.h"
#include <vector>
#include <string>
#include <cctype>
#include <fstream>
#include <algorithm> 
#include <cctype>
#include <locale>
#include "SSISerializable.h"
#include "SSObjMeshMaterial.h"
#include "SSFreqUsedNames.h"
#include "SSCameraManager.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSMathHelper.h"
#include <unordered_set>
#include <unordered_map>

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

				sscanf_s(Line.c_str(), "usemtl %s", buffer, 256);

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

	// @ reorder vertices
	OptimizedGenerateVertices();

	SerializeWriter writer("./Prebaked/sponza.mesh");
	writer << mRealVertexList;
	writer << mRealVertexIndices;
	writer << mMeshSectionList;

	//writer << static_cast<UINT>(mMeshMaterialMap.size());

	//for(auto& kvp : mMeshMaterialMap)
	//{
	//	writer << kvp.first;
	//	writer << kvp.second;		
	//}

	//CreateVertexIndexBuffer();	
	
	return true;
}


bool SSObjMesh::LoadCookedFile(const std::string& filePath)
{
	SerializeReader reader(filePath);
	if(reader.IsGood())
	{
		UINT materialMapSize = 0;

		reader >> mRealVertexList;
		reader >> mRealVertexIndices;
		reader >> mMeshSectionList;
		reader >> materialMapSize;

		for(UINT i = 0; i < materialMapSize; ++i)
		{
			std::string name;
			SSObjMeshMaterial material;

			reader >> name;
			reader >> material;

			//mMeshMaterialMap[name] = material;
		}

		//CreateVertexIndexBuffer();
		
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

SSObjMesh::SSObjMesh()	
{
	mVisible = false;
}

void SSObjMesh::OptimizedGenerateVertices()
{	
	// @check errors 
	check(mVertexIndexList.size() > 0);
	check(mVertexIndexList.size() == mNormalIndexList.size());
	check(mVertexIndexList.size() == mTexcoordIndexList.size());
	check(mVertexIndexList.size() == mTempTangentList.size());

	// @sort mesh section list for low drawcall
	std::sort(mMeshSectionList.begin(), mMeshSectionList.end(), [](SSObjMeshSection a, SSObjMeshSection b)
	{
		//auto strA = a.mSectionName.ToString();
		//auto strB = b.mSectionName.ToString();

		if (a.mSectionName.compare(b.mSectionName) < 0)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	// @create real vertex
	mRealVertexList.resize(mVertexIndexList.size());	

	std::vector<SSObjMeshSection> compressedMeshSection;

	std::unordered_set<std::string> nameSet;
	//
	UINT sortedIndex = 0;

	for (auto& section : mMeshSectionList)
	{
		// new name found
		if (nameSet.count(section.mSectionName) == 0)
		{
			nameSet.insert(section.mSectionName);

			if (compressedMeshSection.size() > 0)
			{
				compressedMeshSection[compressedMeshSection.size() - 1].mEndIndex = sortedIndex;
			}

			compressedMeshSection.push_back(SSObjMeshSection(section.mSectionName, sortedIndex));
		}

		for (UINT i = section.mStartIndex; i < section.mEndIndex; ++i)
		{
			VT_PositionNormalTexcoordTangent v(
				mTempVertexList[mVertexIndexList[i]], mTempNormalList[mNormalIndexList[i]],
				mTempTexCoordList[mTexcoordIndexList[i]], mTempTangentList[i]
			);

			mRealVertexIndices.push_back(sortedIndex);
			mRealVertexList[sortedIndex++] = v;		

			/*sortedIndex++;

			UINT index = 0;
			if(GetSimilarVertexIndex(v, index))
			{
				mRealVertexIndices.push_back(index);
			}
			else
			{
				mRealVertexList.push_back(v);
				auto newIndex = mRealVertexList.size() - 1;
				mVertexCache[v] = newIndex;
				mRealVertexIndices.push_back(newIndex);
			}*/
		}
	}

	if (compressedMeshSection.size() > 0)
	{
		compressedMeshSection[compressedMeshSection.size() - 1].mEndIndex = sortedIndex;
	}

	mMeshSectionList = compressedMeshSection;
	
	// @clear 
	mTempVertexList.clear();
	mTempTangentList.clear();
	mTempTexCoordList.clear();
	mTempNormalList.clear();
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
								
				if (sscanf_s(Line.c_str(), "newmtl %s", buffer, 256) > 0)
				{
					newMaterial = new SSObjMeshMaterial();
					newMaterial->mMaterialName = buffer;
					newMtlStarted = true;
				}
				else if (sscanf_s(Line.c_str(), "map_Ka %s", buffer, 256) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mMetalicMap = buffer;
				}
				else if (sscanf_s(Line.c_str(), "map_Kd %s", buffer, 256) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mDiffuseMap = buffer;
				}
				else if (sscanf_s(Line.c_str(), "bump %s", buffer, 256) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mNormalMap = buffer;
				}
				else if (sscanf_s(Line.c_str(), "map_Ns %s", buffer, 256) > 0)
				{
					check(newMaterial != nullptr);
					newMaterial->mRoughnessMap = buffer;
				}
				else if(sscanf_s(Line.c_str(), "map_d %s", buffer, 256) > 0)
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
					//mMeshMaterialMap[newMaterial->mMaterialName] = *newMaterial;
					delete newMaterial;
					newMaterial = nullptr;
					newMtlStarted = false;
				}
			}
		}

		if (newMtlStarted == true && newMaterial != nullptr)
		{
			//mMeshMaterialMap[newMaterial->mMaterialName] = *newMaterial;
			delete newMaterial;
			newMaterial = nullptr;
		}
	}

	return true;
}


void SSObjMesh::GenerateTangents()
{
	std::vector<XMVECTOR> tangent;
	std::vector<XMVECTOR> bitangent;

	tangent.resize(mTempVertexList.size());
	bitangent.resize(mTempVertexList.size());

	mTempTangentList.resize(mVertexIndexList.size());
	
	// Compute the tangent vector
	for (UINT i = 0; i < mVertexIndexList.size(); i += 3)
	{
		XMVECTOR p1 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i]]);
		XMVECTOR p2 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i+1]]);
		XMVECTOR p3 = XMLoadFloat4(&mTempVertexList[mVertexIndexList[i+2]]);

		UINT texcoord1 = mTexcoordIndexList[i];
		UINT texcoord2 = mTexcoordIndexList[i+1];
		UINT texcoord3 = mTexcoordIndexList[i+2];

		XMVECTOR tc1 = XMLoadFloat2(&mTempTexCoordList[texcoord1]);
		XMVECTOR tc2 = XMLoadFloat2(&mTempTexCoordList[texcoord2]);
		XMVECTOR tc3 = XMLoadFloat2(&mTempTexCoordList[texcoord3]);

		XMFLOAT3 q1; XMStoreFloat3(&q1, p2 - p1);
		XMFLOAT3 q2; XMStoreFloat3(&q2, p3 - p1);

		float s1 = mTempTexCoordList[texcoord2].x - mTempTexCoordList[texcoord1].x;
		float t1 = mTempTexCoordList[texcoord2].y - mTempTexCoordList[texcoord1].y;

		float s2 = mTempTexCoordList[texcoord3].x - mTempTexCoordList[texcoord1].x;		
		float t2 = mTempTexCoordList[texcoord3].y - mTempTexCoordList[texcoord1].y;

		// prevent degeneration
		float r = 1.0f / (s1 * t2 - s2 * t1);

		if (std::isinf(r))
		{
			r = 1 / 0.1f;
		}

		XMFLOAT3 tmptangent;
		tmptangent.x = (t2 * q1.x - t1 * q2.x) * r;
		tmptangent.y = (t2 * q1.y - t1 * q2.y) * r;
		tmptangent.z = (t2 * q1.z - t1 * q2.z) * r;

		XMFLOAT3 tmpbitangent;
		tmpbitangent.x = (s1 * q2.x - s2 * q1.x) * r;
		tmpbitangent.y = (s1 * q2.y - s2 * q1.y) * r;
		tmpbitangent.z = (s1 * q2.z - s2 * q1.z) * r;

		auto tangentVector = XMLoadFloat3(&tmptangent);
		auto bitangentVector = XMLoadFloat3(&tmpbitangent);

		tangent[mVertexIndexList[i]] += tangentVector;
		tangent[mVertexIndexList[i+1]] += tangentVector;
		tangent[mVertexIndexList[i+2]] += tangentVector;

		bitangent[mVertexIndexList[i]] += bitangentVector;
		bitangent[mVertexIndexList[i+1]] += bitangentVector;
		bitangent[mVertexIndexList[i+2]] += bitangentVector;
	}

	XMFLOAT4 lastValidTangent(0,0,0,0);

	for (UINT i = 0; i < mVertexIndexList.size(); ++i)
	{
		auto n1 = mNormalIndexList[i];		

		auto n = DirectX::XMLoadFloat3(&mTempNormalList[n1]);

		auto vertexIndex = mVertexIndexList[i];
		check(vertexIndex < tangent.size());
		auto tangent1 = tangent[mVertexIndexList[i]];
		auto bitangent1 = bitangent[mVertexIndexList[i]];

		// Gram-Schmidt orthogonalize                
		XMFLOAT3 temp;

		XMFLOAT3 normalDotTangent;
		XMStoreFloat3(&normalDotTangent, XMVector3Dot(n, tangent1));

		XMVECTOR computeResult = tangent1 - XMVectorScale(n, normalDotTangent.x);
		XMFLOAT4 errCheck;
		XMStoreFloat4(&errCheck, computeResult);

		bool bValid = true;

		if (errCheck.x == 0 && errCheck.y == 0 && errCheck.z == 0 )
		{
			bValid = false;
		}
				
		XMStoreFloat3(&temp, XMVector3Normalize(computeResult));

		// Store handedness in w                
		XMFLOAT3 dotResult;
		XMStoreFloat3(&dotResult, XMVector3Dot(XMVector3Cross(n, tangent1), bitangent1));

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

	vertexIndexList.push_back(v1-1);  vertexIndexList.push_back(v2-1); vertexIndexList.push_back(v3 - 1);

	texcoordIndexList.push_back(t1-1);  texcoordIndexList.push_back(t2 - 1);  texcoordIndexList.push_back(t3 - 1);

	normalIndexList.push_back(n1-1);  normalIndexList.push_back(n2 - 1);  normalIndexList.push_back(n3 - 1);
}





XMFLOAT4 SSObjMeshParser::ParseVertex(std::string& line)
{
	XMFLOAT4 result;

	sscanf_s(line.c_str(), "v %f %f %f", &result.x, &result.y, &result.z);
	
	// for opengl resource revert z		

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