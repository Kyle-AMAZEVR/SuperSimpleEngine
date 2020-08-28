
#include "Core.h"
#include "SSCube.h"
#include "SSVertexTypes.h"
#include "SSDX11VertexBuffer.h"
#include "FreqUsedConstantBufferTypes.h"


SSCube::SSCube()
{
	CreateRenderingData();
	SetScale(10.f,10.f,10.f);
}

void SSCube::Tick(float delta)
{
	mYaw += 1.f * delta;
}

void SSCube::CreateRenderingData()
{
	std::vector<VT_PositionNormalTexcoordTangent> vertexArray =
	{
		// front
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1), XMFLOAT3(0,0,-1), XMFLOAT2(0,0), XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),  XMFLOAT2(0,1),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,0,1) },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f,1),  XMFLOAT3(0,0,-1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,0,1) },

		// back
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,0) ,XMFLOAT4(0,0,0,1) },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,1) ,XMFLOAT4(0,0,0,1) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,0,1) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,0,1) },

		// top
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,1),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,1),XMFLOAT4(0,0,0,1)},


		// bottom
		{ XMFLOAT4(-1.0f,-1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(1.0f, -1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,1),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,1),XMFLOAT4(0,0,0,1)},

		// left
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,1),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,1),XMFLOAT4(0,0,0,1)},

		// right
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,0),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,1),XMFLOAT4(0,0,0,1)},
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,1),XMFLOAT4(0,0,0,1)},
	};

	std::vector<UINT> indexArray
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// top
		8,9,11,
		11,9,10,

		// bottom
		14,13,12,
		15,14,12,

		// left
		16,17,18,
		16,18,19,

		// right
		20,21,22,
		20,22,23,
	};
	
	mSharedRenderData.IndexData = indexArray;
	mSharedRenderData.VertexType = EVertexType::VT_PNTT;
	mSharedRenderData.PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mSharedRenderData.bHasIndexData = true;
	mSharedRenderData.PNTT_VertexData = std::move(vertexArray);
	mSharedRenderData.VertexShaderName = "GBuffer.vs";
	mSharedRenderData.PixelShaderName = "GBuffer.ps";
	mSharedRenderData.PSTextureMap[SSName("DiffuseTex")] = "./Resource/Tex/rustediron/rustediron2_basecolor.dds";
	mSharedRenderData.PSTextureMap[SSName("NormalTex")] = "./Resource/Tex/rustediron/rustediron2_normal.dds";
	mSharedRenderData.PSTextureMap[SSName("MetalicTex")] = "./Resource/Tex/rustediron/rustediron2_metallic.dds";
	mSharedRenderData.PSTextureMap[SSName("RoughnessTex")] = "./Resource/Tex/rustediron/rustediron2_roughness.dds";
		

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 0; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferProxy proxy{ settings };	
	
	mSharedRenderData.PSConstantBufferMap["TextureExist"] = std::move(proxy);
}

