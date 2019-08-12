#pragma once
#include "SSSceneObject.h"
#include <vector>
#include <map>
#include "DXVertexTypes.h"

bool operator< (const VT_PositionNormalTexcoordTangent& a, const VT_PositionNormalTexcoordTangent& b);

class ENGINE_API SSObjMesh : public SSSceneObject
{
public:
	bool ImportObjFile(const std::string& FilePath, const std::string& MtlFilePath);
	void ImportMtlFile(const std::string& FilePath);
	bool LoadCookedFile(const std::string& filePath);

	virtual void Draw(ID3D11DeviceContext* deviceContext) override;

protected:

	void GenerateTangents();
	void OptimizedGenerateVertices();
	

	XMFLOAT3 mMinPosition;
	XMFLOAT3 mMaxPosition;
	XMFLOAT3 mCenterPosition;

	std::shared_ptr<class SSVertexBuffer> mVB;
	std::shared_ptr<class SSIndexBuffer> mIB;

	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;

	std::vector<UINT> mVertexIndexList;
	std::vector<UINT> mTexcoordIndexList;
	std::vector<UINT> mNormalIndexList;

	bool mHasTexCoord = false;
	bool mHasNormal = false;
	

	std::map<VT_PositionNormalTexcoordTangent, UINT> mVertexCacheMap;
	std::vector<VT_PositionNormalTexcoordTangent> mRealVertexList;
	
	bool GetSimilarVertexIndex(VT_PositionNormalTexcoordTangent& vertex, UINT& index);
};



class SSObjMeshParser
{
public:

	static XMFLOAT4 ParseVertex(std::string& line);
	static XMFLOAT3 ParseNormal(std::string& line);
	static XMFLOAT2 ParseTexcoord(std::string& line);
	
	static void ParseFace(std::string& line, std::vector<UINT>& vertexIndexList, std::vector<UINT>& texcoordIndexList, std::vector<UINT>& normalIndexList);
};