#pragma once
#include "SSSceneObject.h"
#include <vector>


class ENGINE_API SSObjMesh : public SSSceneObject
{
public:
	bool ImportObjFile(const std::string& FilePath, const std::string& MtlFilePath);
	void ImportMtlFile(const std::string& FilePath);

protected:
	XMFLOAT3 mMinPosition;
	XMFLOAT3 mMaxPosition;
	XMFLOAT3 mCenterPosition;

	std::shared_ptr<class SSVertexBuffer> mVB;
	std::shared_ptr<class SSIndexBuffer> mIB;

	std::vector<XMFLOAT4> mTempVertexList;
	std::vector<XMFLOAT3> mTempNormalList;
	std::vector<XMFLOAT2> mTempTexCoordList;
	std::vector<XMFLOAT4> mTempTangentList;
};



class SSObjMeshParser
{
public:
	static XMFLOAT4 ParseFloat4(std::string& line);
	static XMFLOAT3 ParseFloat3(std::string& line);
	static XMFLOAT2 ParseFloat2(std::string& line);
};