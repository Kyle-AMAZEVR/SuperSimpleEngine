

#pragma once
#include "Singleton.h"
#include "SSVertexTypes.h"
#include <vector>


class CORE_API SSSharedRenderData : public Singleton<SSSharedRenderData>
{
public:
	void Initialize();

	std::vector<VT_PositionNormalTexcoordTangent>& GetCubeVertexData();
	std::vector<VT_PositionNormalTexcoordTangent>& GetSphereVertexData();
	std::vector<UINT>& GetCubeIndexData() { return mCubeIndexData; }
	std::vector<VT_PositionTexcoord>& GetScreenBlitVertexData() { return mScreenBlitVertexData; }
	std::vector<UINT>& GetScreenBlitIndexData() {return mScreenBlitIndexData;}
	
protected:
	void CreateCubeVertexData();
	void CreateSphereVertexData();
	void CreateScreenBlitVertexData();

	std::vector<DirectX::XMFLOAT4> GenerateTangents(const std::vector<DirectX::XMFLOAT4>& tempVertexList,
		const std::vector<DirectX::XMFLOAT3>& tempNormalList,
		const std::vector<DirectX::XMFLOAT2>& tempTexCoordList);
	
	std::vector<VT_PositionNormalTexcoordTangent> mCubeVertexData;
	std::vector<VT_PositionNormalTexcoordTangent> mSphereVertexData;
	std::vector<VT_PositionTexcoord> mScreenBlitVertexData;
	std::vector<UINT> mScreenBlitIndexData;

	std::vector<UINT> mCubeIndexData;	
};
