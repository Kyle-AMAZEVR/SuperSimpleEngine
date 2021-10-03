#pragma once
#include <vector>
#include <unordered_map>
#include <map>
#include "SSName.h"
#include "SSVertexTypes.h"
#include "SSObjMeshSection.h"
#include "SSObjMeshMaterial.h"

bool operator< (const VT_PositionNormalTexcoordTangent& a, const VT_PositionNormalTexcoordTangent& b);

class DX11RENDERER_API SSObjMesh : public SSGameObject
{
public:
	SSObjMesh();
	bool ImportObjFile(const std::string& FilePath, const std::string& MtlFilePath);
	void ImportMtlFile(const std::string& FilePath);
	bool LoadCookedFile(const std::string& filePath);

	virtual void Draw(ID3D11DeviceContext* deviceContext) override;

	virtual void Draw(ID3D11DeviceContext* deviceContext, class SSMaterial* material) override;
	virtual void DebugDraw(ID3D11DeviceContext* deviceContext, class SSMaterial* material);

	void SetRoughnessOverride(bool bOverride) { mbRoughnessOverride = bOverride; }
	void SetMetalicOverride(bool bOverride) { mbMetalicOverride = bOverride; }

	bool IsRoughnessOverride() const { return mbRoughnessOverride; }
	bool IsMetalicOverride() const { return mbMetalicOverride; }

protected:

	bool mbRoughnessOverride = false;
	bool mbMetalicOverride = false;

	void GenerateTangents();
	void OptimizedGenerateVertices();
	void CreateVertexIndexBuffer();
	void CreateDebugTBNVertexIndexBuffer();
	
	bool ParseMtlFile(const std::string& filepath);

	XMFLOAT3 mMinPosition;
	XMFLOAT3 mMaxPosition;
	XMFLOAT3 mCenterPosition;

	std::shared_ptr<class SSDX11VertexBuffer> mVB;
	std::shared_ptr<class SSIndexBuffer> mIB;

	std::shared_ptr<class SSDX11VertexBuffer> mTBNDebugVB;
	std::shared_ptr<class SSIndexBuffer> mTBNDebugIB;

	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;

	std::vector<UINT> mVertexIndexList;
	std::vector<UINT> mTexcoordIndexList;
	std::vector<UINT> mNormalIndexList;

	bool mHasTexCoord = false;
	bool mHasNormal = false;
	
	std::vector<VT_PositionNormalTexcoordTangent> mRealVertexList;
	std::vector<UINT> mRealVertexIndices;

	std::vector<SSObjMeshSection> mMeshSectionList;
	std::map<std::string, SSObjMeshMaterial> mMeshMaterialMap;
	
	//bool GetSimilarVertexIndex(const VT_PositionNormalTexcoordTangent& vertex, UINT& index);
};


class SSObjMeshParser
{
public:

	static XMFLOAT4 ParseVertex(std::string& line);
	static XMFLOAT3 ParseNormal(std::string& line);
	static XMFLOAT2 ParseTexcoord(std::string& line);
	
	static void ParseFace(std::string& line, std::vector<UINT>& vertexIndexList, std::vector<UINT>& texcoordIndexList, std::vector<UINT>& normalIndexList);
};