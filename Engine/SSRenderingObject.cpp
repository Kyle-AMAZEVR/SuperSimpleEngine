
#include "Core.h"
#include "SSGameObject.h"
#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSCameraManager.h"
#include "SSMaterial.h"

SSRenderingObject::SSRenderingObject(SSGameObject* pGameObject)
	: mpGameObject(pGameObject)
{
	mRenderData = pGameObject->GetRenderData();
	
	mVertexBuffer = new SSDX11VertexBuffer();

	if(mRenderData.VertexType == EVertexType::VT_PNTT)
	{
		mVertexBuffer->SetVertexBufferData(mRenderData.PNTT_VertexData);
	}
	else if(mRenderData.VertexType == EVertexType::VT_PNT)
	{
		mVertexBuffer->SetVertexBufferData(mRenderData.PNT_VertexData);
	}		

	if(mRenderData.bHasIndexData)
	{
		mIndexBuffer = new SSIndexBuffer();
		mIndexBuffer->SetIndexBufferData(mRenderData.IndexData);
	}

	shared_ptr<SSVertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);
	
	shared_ptr<SSPixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);

	mMaterial = new SSMaterial(vs.get(), ps.get());	
}


SSRenderingObject::~SSRenderingObject()
{
	if(mVertexBuffer)
	{		
		mVertexBuffer->Destroy();
		delete mVertexBuffer;
		mVertexBuffer = nullptr;
	}

	if(mIndexBuffer)
	{
		mIndexBuffer->Destroy();
		delete mIndexBuffer;
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

	auto stride = mVertexBuffer->GetStride();
	UINT offset = 0;
	// set vertex buffer
	deviceContext->IASetVertexBuffers(0, 1, mVertexBuffer->GetDX11BufferPointerRef(), &stride, &offset);

	if (mRenderData.bHasIndexData)
	{
		// set indexbuffer
		deviceContext->IASetIndexBuffer(mIndexBuffer->GetDX11BufferPointer(), DXGI_FORMAT_R32_UINT, 0);
	}

	// 
	mMaterial->SetCurrent();
	
	// 
	deviceContext->IASetPrimitiveTopology(mRenderData.PrimitiveType);
	
	// setup vertex shader	
	// set model, view, proj matrix
	mMaterial->SetVSConstantBufferData(deviceContext, ModelName, mpGameObject->GetModelTransform());
	mMaterial->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	mMaterial->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	for(auto& [k,v] : mRenderData.VSConstantBufferMap)
	{
		
	}


	// if have index buffer
	if(mRenderData.bHasIndexData)
	{		
		deviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);
	}
	else
	{
		deviceContext->Draw(mVertexBuffer->GetVertexCount(), 0);
	}


	
}
