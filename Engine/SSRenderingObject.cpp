
#include "Core.h"
#include "SSGameObject.h"
#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSCameraManager.h"
#include "SSMaterial.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"
#include "SSSamplerManager.h"

SSRenderingObject::SSRenderingObject(SSObjectBase* pObject)
	: mpObject(pObject)
{
	mRenderData = pObject->GetRenderData();
	mVertexData = pObject->GetVertexData();
	
	mVertexBuffer = std::make_shared<SSDX11VertexBuffer>();

	if(mVertexData.VertexType == EVertexType::VT_PNTT)
	{
		mVertexBuffer->SetVertexBufferData(mVertexData.PNTT_VertexData);
	}
	else if(mVertexData.VertexType == EVertexType::VT_PNT)
	{
		mVertexBuffer->SetVertexBufferData(mVertexData.PNT_VertexData);
	}
	else if(mVertexData.VertexType == EVertexType::VT_PT)
	{
		mVertexBuffer->SetVertexBufferData(mVertexData.PT_VertexData);
	}

	if(mVertexData.bHasIndexData)
	{
		mIndexBuffer = std::make_shared<SSIndexBuffer>();
		mIndexBuffer->SetIndexBufferData(mVertexData.IndexData);
	}

	shared_ptr<SSVertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);	
	shared_ptr<SSPixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);

	mMaterial = new SSMaterial(vs, ps);
}


SSRenderingObject::~SSRenderingObject()
{
	if(mVertexBuffer)
	{		
		mVertexBuffer->Destroy();
		mVertexBuffer.reset();
		mVertexBuffer = nullptr;
	}

	if(mIndexBuffer)
	{
		mIndexBuffer->Destroy();
		mIndexBuffer.reset();
		mIndexBuffer = nullptr;
	}

	if(mMaterial != nullptr)
	{
		delete mMaterial;
		mMaterial = nullptr;
	}
}

void SSRenderingObject::Draw(ID3D11DeviceContext* deviceContext)
{	
	// 
	mMaterial->SetCurrent();

	auto stride = mVertexBuffer->GetStride();
	UINT offset = 0;
	// set vertex buffer
	deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetDX11BufferPointerRef(), &stride, &offset);

	if (mVertexData.bHasIndexData)
	{
		// set indexbuffer
		deviceContext->IASetIndexBuffer(mIndexBuffer->GetDX11BufferPointer(), DXGI_FORMAT_R32_UINT, 0);
	}
		
	// 
	deviceContext->IASetPrimitiveTopology(mVertexData.PrimitiveType);
	
	// setup vertex shader	
	// set model, view, proj matrix
	mMaterial->SetVSConstantBufferData(deviceContext, ModelName, XMMatrixTranspose(mpObject->GetModelTransform()));
	mMaterial->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	mMaterial->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	// set vertex shader constants
	for(auto& [k,v] : mRenderData.VSConstantBufferMap)
	{
		mMaterial->SetVSConstantBufferProxyData(deviceContext, k, v);
	}

	// set pixel shader constants	
	for(auto& [k,v] : mRenderData.PSConstantBufferMap)
	{
		mMaterial->SetPSConstantBufferProxyData(deviceContext, k, v);
	}

	// @ set pixel shader texture
	for (auto& [name, texture] : mRenderData.PSTextureMap)
	{
		shared_ptr<SSTexture2D> resource = SSTextureManager::Get().LoadTexture2D(deviceContext, texture);
		mMaterial->SetPSTexture(deviceContext, name, resource.get());
	}

	// @ set vertex shader texture 
	for (auto& [name, texture] : mRenderData.VSTextureMap)
	{
		shared_ptr<SSTexture2D> resource = SSTextureManager::Get().LoadTexture2D(deviceContext, texture);
		mMaterial->SetVSTexture(deviceContext, name, resource.get());
	}


	ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();

	mMaterial->SetPSSampler("DefaultTexSampler", sampler);
	
	// if have index buffer
	if(mVertexData.bHasIndexData)
	{
	    if(mVertexData.bHasInstancedData)
        {
	        //deviceContext->DrawIndexedInstanced();
        }
	    else
        {
            deviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);
        }
	}
	else
	{
	    if(mVertexData.bHasInstancedData)
        {
	        //deviceContext->DrawInstanced()
        }
	    else
        {
            deviceContext->Draw(mVertexBuffer->GetVertexCount(), 0);
        }
	}



	mMaterial->ReleaseCurrent();
}

void SSRenderingObject::SetPSTexture(SSName name, SSName textureName)
{
	mRenderData.PSTextureMap[name] = textureName;
}

void SSRenderingObject::SetVSTexture(SSName name, SSName textureName)
{
	mRenderData.VSTextureMap[name] = textureName;
}

void SSRenderingObject::SetVSConstantBufferData(SSName name, class SSConstantBufferProxy&& buffer)
{
	mRenderData.VSConstantBufferMap[name] = std::move(buffer);
}

void SSRenderingObject::SetPSConstantBufferData(SSName name, class SSConstantBufferProxy&& buffer)
{
	mRenderData.PSConstantBufferMap[name] = std::move(buffer);
}

void SSRenderingObject::SetVSConstantBufferData(SSName name, const SSConstantBufferProxy& buffer)
{
	mRenderData.VSConstantBufferMap[name] = buffer;
}

void SSRenderingObject::SetPSConstantBufferData(SSName name, const SSConstantBufferProxy& buffer)
{
	mRenderData.PSConstantBufferMap[name] = buffer;
}