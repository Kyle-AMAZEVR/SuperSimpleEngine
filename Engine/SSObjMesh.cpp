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
#include "SSIndexBuffer.h"

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
				mTempVertexList.push_back(SSObjMeshParser::ParseFloat3ToFloat4(Line));
			}
			else if (Line[0] == 'v' && Line[1] == 't')
			{
				//@ Texcoord
				mTempTexCoordList.push_back(std::move(SSObjMeshParser::ParseFloat3ToFloat2(Line)));
				mHasTexCoord = true;
			}
			else if (Line[0] == 'v' && Line[1] == 'n')
			{
				//@ Normal
				mTempNormalList.push_back(std::move(SSObjMeshParser::ParseFloat3(Line)));
				mHasNormal = true;
			}
			else if (Line[0] == 'f' && std::isspace(Line[1]))
			{
				//@Face Info
				SSObjMeshParser::ParseFace(Line, mVertexIndexList, mTexcoordIndexList, mNormalIndexList);
			}
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
	for(UINT i = 0; i < mVertexIndexList.size(); ++i)
	{
		VT_PositionNormalTexcoordTangent v
		{
			mTempVertexList[mVertexIndexList[i]],
			mTempNormalList[mVertexIndexList[i]],
			mTempTexCoordList[mVertexIndexList[i]],
			mTempTangentList[mVertexIndexList[i]],
		};
		mRealVertexList.push_back(v);
	}

	// @clear 
	mTempVertexList.clear();
	mTempTangentList.clear();
	mTempTexCoordList.clear();
	mTempNormalList.clear();

	mVB = std::make_shared<SSVertexBuffer>();
	mVB->SetVertexBufferData(mRealVertexList);
	
	return true;
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
	deviceContext->IASetIndexBuffer(mIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	deviceContext->DrawIndexed(mIB->GetIndexCount(), 0, 0);
}

void SSObjMesh::OptimizedGenerateVertices()
{	
	
}

bool SSObjMesh::GetSimilarVertexIndex(VT_PositionNormalTexcoordTangent& vertex, UINT& index)
{
	if (mVertexCacheMap.count(vertex) > 0)
	{
		index = mVertexCacheMap[vertex];
		return true;
	}
	else
	{
		index = 0;
		return false;
	}
}


void SSObjMesh::GenerateTangents()
{
	std::vector<XMVECTOR> tan1Accum;
	std::vector<XMVECTOR> tan2Accum;

	for (UINT i = 0; i < mTempVertexList.size(); ++i)
	{
		tan1Accum.push_back(XMVECTOR());
		tan2Accum.push_back(XMVECTOR());
	}

	for (UINT i = 0; i < mVertexIndexList.size(); i++)
	{
		mTempTangentList.push_back(XMFLOAT4(0, 0, 0, 0));
	}

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
		auto n = DirectX::XMLoadFloat3(&mTempNormalList[mNormalIndexList[i]]);
		auto t1 = tan1Accum[mVertexIndexList[i]];
		auto t2 = tan2Accum[mVertexIndexList[i]];

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
	size_t index = 0;

	std::vector<size_t> indexList;

	bool bPrevCharIsSpace = false;

	for (auto i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) && bPrevCharIsSpace == false)
		{
			indexList.push_back(i);
			bPrevCharIsSpace = true;
		}
		else
		{
			bPrevCharIsSpace = false;
		}
	}

	check(indexList.size() >= 3);	

	auto token1 = line.substr(indexList[0], indexList[1] - indexList[0]);
	trim(token1);

	auto token2 = line.substr(indexList[1], indexList[2] - indexList[1]);
	trim(token2);

	auto token3 = line.substr(indexList[2], line.size() - indexList[2]);
	trim(token3);

	std::vector<size_t> seperatorList;

	for(auto i = 0; i < token1.size(); ++i)
	{
		if(token1[i] == '/')
		{
			seperatorList.push_back(i);
		}
	}

	check(seperatorList.size() == 2);
	
	auto vertexIndex = atoi(token1.substr(0, seperatorList[0]).c_str()) - 1;
	vertexIndexList.push_back(vertexIndex);
	auto texcoordIndex = atoi(token1.substr(seperatorList[0] + 1, seperatorList[1] - seperatorList[0]).c_str()) - 1;
	texcoordIndexList.push_back(texcoordIndex);
	auto normalIndex = atoi(token1.substr(seperatorList[1] + 1, token1.size() - seperatorList[1]).c_str()) - 1;
	normalIndexList.push_back(normalIndex);

	seperatorList.clear();

	for (auto i = 0; i < token2.size(); ++i)
	{
		if (token2[i] == '/')
		{
			seperatorList.push_back(i);
		}
	}

	check(seperatorList.size() == 2);

	vertexIndexList.push_back(atoi(token2.substr(0, seperatorList[0]).c_str()) - 1);
	texcoordIndexList.push_back(atoi(token2.substr(seperatorList[0] + 1, seperatorList[1] - seperatorList[0]).c_str()) - 1);
	normalIndexList.push_back(atoi(token2.substr(seperatorList[1] + 1, token1.size() - seperatorList[1]).c_str()) - 1);

	seperatorList.clear();

	for (auto i = 0; i < token3.size(); ++i)
	{
		if (token3[i] == '/')
		{
			seperatorList.push_back(i);
		}
	}

	check(seperatorList.size() == 2);
	vertexIndexList.push_back(atoi(token3.substr(0, seperatorList[0]).c_str()) - 1);
	texcoordIndexList.push_back(atoi(token3.substr(seperatorList[0] + 1, seperatorList[1] - seperatorList[0]).c_str()) - 1);
	normalIndexList.push_back(atoi(token3.substr(seperatorList[1] + 1, token1.size() - seperatorList[1]).c_str()) - 1);
}





XMFLOAT4 SSObjMeshParser::ParseFloat3ToFloat4(std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,
		DIGIT,
	};

	std::vector<size_t> indexList;

	bool bPrevCharIsSpace = false;

	for (auto i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) && bPrevCharIsSpace == false)
		{
			indexList.push_back(i);
			bPrevCharIsSpace = true;
		}
		else
		{
			bPrevCharIsSpace = false;
		}
	}

	check(indexList.size() == 3);

	XMFLOAT4 result;

	sscanf_s(line.substr(indexList[0], indexList[1] - indexList[0]).c_str(), "%f", &result.x);
	sscanf_s(line.substr(indexList[1], indexList[2] - indexList[1]).c_str(), "%f", &result.y);
	sscanf_s(line.substr(indexList[2], line.size() - indexList[2]).c_str(), "%f", &result.z);
	result.w = 1;

	return result;
}

XMFLOAT3 SSObjMeshParser::ParseFloat3(std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,
		DIGIT,
	};

	std::vector<size_t> indexList;

	bool bPrevCharIsSpace = false;
	
	for (auto i = 0; i < line.size(); ++i)
	{
		if(isspace(line[i]) && bPrevCharIsSpace == false)
		{
			indexList.push_back(i);
			bPrevCharIsSpace = true;
		}
		else
		{
			bPrevCharIsSpace = false;
		}
	}

	check(indexList.size() == 3);

	XMFLOAT3 result;

	auto str1 = line.substr(indexList[0], indexList[1] - indexList[0]);
	trim(str1);
	
	auto str2 = line.substr(indexList[1], indexList[2] - indexList[1]);
	trim(str2);

	auto str3 = line.substr(indexList[2], line.size() - indexList[2]);
	trim(str3);
	
	sscanf_s(str1.c_str(), "%f", &result.x);
	sscanf_s(str2.c_str(), "%f", &result.y);
	sscanf_s(str3.c_str(), "%f", &result.z);

	return result;
}

 XMFLOAT2 SSObjMeshParser::ParseFloat3ToFloat2(std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,
		DIGIT,
	};

	std::vector<size_t> indexList;

	bool bPrevCharIsSpace = false;

	for (auto i = 0; i < line.size(); ++i)
	{
		if (isspace(line[i]) && bPrevCharIsSpace == false)
		{
			indexList.push_back(i);
			bPrevCharIsSpace = true;
		}
		else
		{
			bPrevCharIsSpace = false;
		}
	}

	check(indexList.size() == 3);

	XMFLOAT2 result;

	auto str1 = line.substr(indexList[0], indexList[1] - indexList[0]);
	trim(str1);
	auto str2 = line.substr(indexList[1], indexList[2] - indexList[1]);
	trim(str2);

	sscanf_s(str1.c_str(), "%f", &result.x);
	sscanf_s(str2.c_str(), "%f", &result.y);

	return result;
}