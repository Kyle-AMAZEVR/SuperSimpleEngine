
#include "SSCore.h"
#include "SSObjectBase.h"

using namespace DirectX;

SSObjectBase::SSObjectBase()
{
	mPosition = XMFLOAT3(0, 0, 0);
	mScale = XMFLOAT3(1, 1, 1);
}

SSObjectBase::~SSObjectBase()
{
}

void SSObjectBase::SetVSConstantBufferData(SSName name, SSConatantBufferData&& buffer)
{
	mRenderData.VSConstantBufferMap[name] = std::move(buffer);
}

void SSObjectBase::SetPSConstantBufferData(SSName name, SSConatantBufferData&& buffer)
{
	mRenderData.PSConstantBufferMap[name] = std::move(buffer);
}

void SSObjectBase::SetVertexShader(SSName vs)
{
	mRenderData.VertexShaderName = vs;
}

void SSObjectBase::SetPixelShader(SSName ps)
{
	mRenderData.PixelShaderName = ps;
}

void SSObjectBase::SetPSTexture(SSName paramName, SSName texture)
{
	mRenderData.PSTextureMap[paramName] = texture;
}

void SSObjectBase::SetVSTexture(SSName paramName, SSName texture)
{
	mRenderData.VSTextureMap[paramName] = texture;
}

const SSMeshVertexData& SSObjectBase::GetVertexData()
{
	return mVertexData;
}

std::vector<SSMeshElementRenderData> SSObjectBase::GetMeshElementRenderData()
{
	std::vector<SSMeshElementRenderData> dummy;
	return std::move(dummy);
}

const SSMeshRenderData& SSObjectBase::GetRenderData()
{
	return mRenderData;
}

const SSInstanceData& SSObjectBase::GetInstancedData()
{
    return mInstanceData;
}

XMMATRIX SSObjectBase::GetModelTransform()
{
	return XMMatrixScaling(mScale.x, mScale.y, mScale.z) * XMMatrixRotationY(mYaw) * XMMatrixTranslation(mPosition.x, mPosition.y, mPosition.z);
}

void SSObjectBase::SetPositionX(float x)
{
	mPosition.x = x;
}

void SSObjectBase::SetPositionY(float y)
{
	mPosition.y = y;
}

void SSObjectBase::SetPositionZ(float z)
{
	mPosition.z = z;
}

void SSObjectBase::SetScaleX(float x)
{
	mScale.x = x;
}

void SSObjectBase::SetScaleY(float y)
{
	mScale.y = y;
}

void SSObjectBase::SetScaleZ(float z)
{
	mScale.z = z;
}

void SSObjectBase::SetPosition(float x, float y, float z)
{
	mPosition.x = x; mPosition.y = y; mPosition.z = z;
}
void SSObjectBase::SetScale(float x, float y, float z)
{
	mScale.x = x; mScale.y = y; mScale.z = z;
}
