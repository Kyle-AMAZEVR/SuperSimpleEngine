
#include "SSCube.h"
#include "SSVertexTypes.h"
#include "SSSharedRenderData.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSName.h"

SSCube::SSCube()
{
	CreateVertexData();
	CreateRenderData();
	SetScale(5.f,5.f,5.f);
	SetPositionZ(1);	
}

void SSCube::Tick(float delta)
{
	mYaw += 3.f * delta;
}

void SSCube::CreateRenderData()
{		
	mRenderData.VertexShaderName = "GBuffer.vs";
	mRenderData.PixelShaderName = "GBuffer.ps";
	mRenderData.PSTextureMap[SSName("DiffuseTex")] = "./Resource/Tex/rustediron/rustediron2_basecolor.dds";
	mRenderData.PSTextureMap[SSName("NormalTex")] = "./Resource/Tex/rustediron/rustediron2_normal.dds";
	mRenderData.PSTextureMap[SSName("MetalicTex")] = "./Resource/Tex/rustediron/rustediron2_metallic.dds";
	mRenderData.PSTextureMap[SSName("RoughnessTex")] = "./Resource/Tex/rustediron/rustediron2_roughness.dds";		

	SSAlignedCBuffer<int, int, int, int, int> settings;
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferProxy proxy{ settings };	
	
	mRenderData.PSConstantBufferMap[SSName("TextureExist")] = std::move(proxy);
}

void SSCube::CreateVertexData()
{		
	mVertexData.IndexData = SSSharedRenderData::Get().GetCubeIndexData();
	mVertexData.VertexType = EVertexType::VT_PNTT;
	mVertexData.PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mVertexData.bHasIndexData = true;	
	mVertexData.PNTT_VertexData = SSSharedRenderData::Get().GetCubeVertexData();
}

