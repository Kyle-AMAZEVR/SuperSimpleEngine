
#include "SSSphere.h"
#include <cmath>
#include "SSVertexTypes.h"
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
#include "SSScreenBlit.h"
#include "SSTexture2D.h"
#include "SSSharedRenderData.h"

SSSphere::SSSphere(UINT sector, UINT stack, float radius)
	: mSectorCount(sector), mStackCount(stack), mRadius(radius)
{	
	CreateVertexData();
	CreateRenderData();	
}

void SSSphere::CreateVertexData()
{
	mVertexData.bHasIndexData = false;
    mVertexData.PNTT_VertexData = SSSharedRenderData::Get().GetSphereVertexData();
	mVertexData.VertexType = EVertexType::VT_PNTT;
}

void SSSphere::CreateRenderData()
{
	SSAlignedCBuffer<int, int, int, int, int> settings;	
	
	mRenderData.VertexShaderName = "GBuffer.vs";
	mRenderData.PixelShaderName = "GBuffer.ps";
	mRenderData.PSTextureMap[SSName("DiffuseTex")] = "./Resource/Tex/rustediron/rustediron2_basecolor.dds";
	mRenderData.PSTextureMap[SSName("NormalTex")] = "./Resource/Tex/rustediron/rustediron2_normal.dds";
	mRenderData.PSTextureMap[SSName("MetalicTex")] = "./Resource/Tex/rustediron/rustediron2_metallic.dds";
	mRenderData.PSTextureMap[SSName("RoughnessTex")] = "./Resource/Tex/rustediron/rustediron2_roughness.dds";
		
	settings.value1 = 1; //metalic
	settings.value2 = 0; //mask
	settings.value3 = 1; //normal
	settings.value4 = 1; // roghness
	settings.value5 = 1; // diffuse

	SSConstantBufferProxy proxy{ settings };

	mRenderData.PSConstantBufferMap[SSName("TextureExist")] = std::move(proxy);
}

void SSSphere::Tick(float delta)
{
    mYaw += 1.f * delta;
}

std::vector< VT_PositionNormalTexcoordTangent > SSSphere::mVertexArray;

SSPBRSphere::SSPBRSphere()
: SSSphere(25,25,10)
{
    mNormalTexName = "./Resource/Tex/metalgrid/metalgrid4_normal-dx.dds";
    mDiffuseTexName = "./Resource/Tex/metalgrid/metalgrid4_basecolor.dds";
    mMetalTexName = "./Resource/Tex/metalgrid/metalgrid4_metallic.dds";
    mRoughTexName = "./Resource/Tex/metalgrid/metalgrid4_roughness.dds";

    CreateRenderData();
}

SSPBRSphere::SSPBRSphere(SSName diffuseTexName, SSName normalTexName, SSName metalTexName, SSName roughTexName)
	: SSSphere(25, 25, 10.f)
{
	mDiffuseTexName = diffuseTexName;
	mNormalTexName = normalTexName;
	mMetalTexName = metalTexName;
	mRoughTexName = roughTexName;

	CreateRenderData();
}

void SSPBRSphere::CreateRenderData()
{
    mRenderData.VertexShaderName = "GBuffer.vs";
    mRenderData.PixelShaderName = "GBufferNoMask.ps";

    mRenderData.PSTextureMap[SSName("DiffuseTex")] = mDiffuseTexName;
    mRenderData.PSTextureMap[SSName("NormalTex")] = mNormalTexName;
    mRenderData.PSTextureMap[SSName("MetalicTex")] = mMetalTexName;
    mRenderData.PSTextureMap[SSName("RoughnessTex")] = mRoughTexName;
}


SSInstancedSphere::SSInstancedSphere(UINT instanceCount, UINT sector, UINT stack, float radius)
: SSSphere(sector,stack, radius)
{
    CreateInstanceData();
}

void SSInstancedSphere::CreateInstanceData()
{

}