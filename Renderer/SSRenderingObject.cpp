
#include "SSRendererModulePCH.h"
#include "SSObjectBase.h"
#include "SSRenderingObject.h"
#include "SSDX11VertexBuffer.h"
#include "SSDX11IndexBuffer.h"
#include "SSShaderManager.h"
#include "SSFreqUsedNames.h"
#include "SSMaterial.h"
#include "SSTextureManager.h"
#include "SSSamplerManager.h"
#include "SSCameraManager.h"
#include "SSRenderCommand.h"
#include "SSShader.h"

SSRenderingObject::SSRenderingObject(SSObjectBase* pObject)
	: mpObject(pObject)
{
	mRenderData = pObject->GetRenderData();
	mVertexData = pObject->GetVertexData();
	
	mVertexBuffer = GetDX11Device()->CreateVertexBuffer(mVertexData.Stride, mVertexData.Count, mVertexData.DataPtr);
	
	// setup index data
	if(mVertexData.bHasIndexData)
	{	
		mIndexBuffer = GetDX11Device()->CreateIndexBuffer(mVertexData.IndexData);
	}

	shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);	
	shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);

	mMaterial = new SSMaterial(vs, ps);

	CreateRenderCmdList();
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

void SSRenderingObject::CreateRenderCmdList()
{
	shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);
	shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);
	
	RenderCmdList.push_back(new SSRenderCmdSetVS(vs));
	RenderCmdList.push_back(new SSRenderCmdSetPS(ps));	
	RenderCmdList.push_back(new SSRenderCmdSetVertexBuffer(mVertexBuffer));

	if (mVertexData.bHasIndexData)
	{
		RenderCmdList.push_back(new SSRenderCmdSetIndexBuffer(mIndexBuffer));
	}

	// set vertex shader constants
	for (auto& [k, v] : mRenderData.VSConstantBufferMap)
	{		
		const int SlotIndex = vs->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSDX11ConstantBuffer* ConstantBuffer = vs->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);			
			RenderCmdList.push_back(new SSRenderCmdCopyCBuffer(ConstantBuffer));
			RenderCmdList.push_back(new SSRenderCmdSetVSCBuffer(vs.get(), vs->GetConstantBuffer(k), SlotIndex));
		}
	}

	// set pixel shader constants
	for (auto& [k, v] : mRenderData.PSConstantBufferMap)
	{		
		const int SlotIndex = ps->GetConstantBufferSlotIndex(k);
		if (SlotIndex != -1)
		{
			SSDX11ConstantBuffer* ConstantBuffer = ps->GetConstantBuffer(k);
			ConstantBuffer->SetBufferData(v);
			RenderCmdList.push_back(new SSRenderCmdCopyCBuffer(ConstantBuffer));
			RenderCmdList.push_back(new SSRenderCmdSetPSCBuffer(ps.get(), ps->GetConstantBuffer(k), SlotIndex));
		}
	}	

	// @ set pixel shader texture
	for (auto& [name, texture] : mRenderData.PSTextureMap)
	{
		const int SlotIndex = ps->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetPSTexture(ps.get(), resource.get(), SlotIndex));
	}

	// @ set vertex shader texture 
	for (auto& [name, texture] : mRenderData.VSTextureMap)
	{
		const int SlotIndex = vs->GetTextureSlotIndex(name);
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(GetDX11Device()->GetDeviceContext(), texture);
		RenderCmdList.push_back(new SSRenderCmdSetVSTexture(vs.get(), resource.get(), SlotIndex));
	}

	if (mVertexData.bHasIndexData)
	{
		//
		RenderCmdList.push_back(new SSRenderCmdDrawIndexed(mIndexBuffer));
	}
	else
	{
		RenderCmdList.push_back(new SSRenderCmdDrawWithoutIndex(mVertexBuffer->GetVertexCount()));
	}
}

void SSRenderingObject::Draw(ID3D11DeviceContext* deviceContext)
{	
	/*shared_ptr<SSDX11VertexShader> vs = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);
	shared_ptr<SSDX11PixelShader> ps = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);

	SSDX11ConstantBuffer* ModelCBuffer = vs->GetConstantBuffer(ModelName.ToString());
	XMMATRIX ModelMatrix = XMMatrixTranspose(mpObject->GetModelTransform());
	ModelCBuffer->SetBufferData((void*)&ModelMatrix, sizeof(XMMATRIX));
	ModelCBuffer->SubmitDataToDevice(deviceContext);
	deviceContext->VSSetConstantBuffers(ModelCBuffer->GetBufferIndex(), 1, (ID3D11Buffer* const*)ModelCBuffer->GetBufferPointerRef());

	SSDX11ConstantBuffer* ViewCBuffer = vs->GetConstantBuffer(ViewName.ToString());
	XMMATRIX ViewMatrix = XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView());
	ViewCBuffer->SetBufferData((void*)&ModelMatrix, sizeof(XMMATRIX));
	ViewCBuffer->SubmitDataToDevice(deviceContext);
	deviceContext->VSSetConstantBuffers(ViewCBuffer->GetBufferIndex(), 1, (ID3D11Buffer* const*)ViewCBuffer->GetBufferPointerRef());

	SSDX11ConstantBuffer* ProjCBuffer = vs->GetConstantBuffer(ProjName.ToString());
	XMMATRIX ProjMatrix = XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj());
	ProjCBuffer->SetBufferData((void*)&ProjMatrix, sizeof(XMMATRIX));
	ProjCBuffer->SubmitDataToDevice(deviceContext);
	deviceContext->VSSetConstantBuffers(ProjCBuffer->GetBufferIndex(), 1, (ID3D11Buffer* const*)ProjCBuffer->GetBufferPointerRef());

	for (auto* Cmd : RenderCmdList)
	{
		Cmd->Execute(deviceContext);
	}

	return;
	*/
	// 
	mMaterial->SetCurrent();

	//	    
	UINT stride[1]{mVertexBuffer->GetStride()};
    UINT offset[1]{0};
    ID3D11Buffer* buffer[1]{(ID3D11Buffer*)mVertexBuffer->GetBufferPointer()};
    // set vertex buffer
    deviceContext->IASetVertexBuffers(0, 1, buffer, stride, offset);    

	if (mVertexData.bHasIndexData)
	{
		// set indexbuffer
		deviceContext->IASetIndexBuffer((ID3D11Buffer*)mIndexBuffer->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);
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
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(deviceContext, texture);
		mMaterial->SetPSTexture(deviceContext, name, resource.get());
	}

	// @ set vertex shader texture 
	for (auto& [name, texture] : mRenderData.VSTextureMap)
	{
		shared_ptr<SSDX11Texture2D> resource = SSTextureManager::Get().LoadTexture2D(deviceContext, texture);
		mMaterial->SetVSTexture(deviceContext, name, resource.get());
	}


	ID3D11SamplerState* sampler = SSSamplerManager::Get().GetDefaultSamplerState();

	mMaterial->SetPSSampler("DefaultTexSampler", sampler);
	
	// if have index buffer
	if(mVertexData.bHasIndexData)
	{	   
        deviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);       
	}
	else
	{	    
        deviceContext->Draw(mVertexBuffer->GetVertexCount(), 0);        
	}

	mMaterial->ReleaseCurrent();
	
}
