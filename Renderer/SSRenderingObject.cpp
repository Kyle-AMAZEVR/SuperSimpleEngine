
#include "SSRendererModulePCH.h"

#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"

#include "SSMaterial.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"
#include "SSSamplerManager.h"
#include "SSCameraManager.h"

SSRenderingObject::SSRenderingObject(SSObjectBase* pObject)
	: mpObject(pObject)
{
	mRenderData = pObject->GetRenderData();
	mVertexData = pObject->GetVertexData();
	
	mVertexBuffer = std::make_shared<SSDX11VertexBuffer>();

	// setup vertex data
	switch(mVertexData.VertexType)
    {
        case EVertexType::VT_PNT:
            mVertexBuffer->SetVertexBufferData(mVertexData.PNT_VertexData);
            break;
        case EVertexType::VT_PNTT:
            mVertexBuffer->SetVertexBufferData(mVertexData.PNTT_VertexData);
            break;
        case EVertexType::VT_PT:
            mVertexBuffer->SetVertexBufferData(mVertexData.PT_VertexData);
            break;
    }
	// setup instance data
	if(mVertexData.bHasInstancedData)
    {
        mInstancedVertexBuffer = std::make_shared<SSDX11InstancedVertexBuffer>();

        switch(mVertexData.InstanceData.InstanceDataType)
        {
            case EInstanceDataType::IDT_FLOAT2:
                mInstancedVertexBuffer->SetInstanceVertexBufferData(mVertexData.InstanceData.Float2InstancedData);
                break;
            case EInstanceDataType::IDT_FLOAT3:
                mInstancedVertexBuffer->SetInstanceVertexBufferData(mVertexData.InstanceData.Float3InstancedData);
                break;
            case EInstanceDataType::IDT_FLOAT4:
                mInstancedVertexBuffer->SetInstanceVertexBufferData(mVertexData.InstanceData.Float4InstancedData);
                break;
        }
    }

	// setup index data
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
		mVertexBuffer.reset();
		mVertexBuffer = nullptr;
	}

	if(mIndexBuffer)
	{		
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

	//
	if(mVertexData.bHasInstancedData == false)
    {
	    UINT stride[1]{mVertexBuffer->GetStride()};
        UINT offset[1]{0};
        ID3D11Buffer* buffer[1]{mVertexBuffer->GetDX11BufferPointer()};
        // set vertex buffer
        deviceContext->IASetVertexBuffers(0, 1, buffer, stride, offset);
    }
	else
    {
	    UINT stride[2]{mVertexBuffer->GetStride(), mInstancedVertexBuffer->GetStride()};
	    UINT offset[2]{0,0};
	    ID3D11Buffer* buffer[2] {mVertexBuffer->GetDX11BufferPointer(), mInstancedVertexBuffer->GetDX11BufferPointer()};
	    deviceContext->IASetVertexBuffers(0, 2, buffer, stride, offset);
    }

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