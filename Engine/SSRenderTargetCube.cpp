
#include "SSCommon.h"
#include "SSRenderTargetCube.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSMaterial.h"
#include "SSFreqUsedNames.h"
#include "SSCameraManager.h"
#include "SSShaderManager.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"
#include "SSSamplerManager.h"

using namespace DirectX;

SSRenderTargetCube::SSRenderTargetCube(SSName vs, SSName ps)
{
	CreateVertexData();

	mRenderData.VertexShaderName = vs;
	mRenderData.PixelShaderName = ps;
	
	mVertexBuffer = std::make_shared<SSDX11VertexBuffer>();

	if (mVertexData.VertexType == EVertexType::VT_PNTT)
	{
		mVertexBuffer->SetVertexBufferData(mVertexData.PNTT_VertexData);
	}
	else if (mVertexData.VertexType == EVertexType::VT_PNT)
	{
		mVertexBuffer->SetVertexBufferData(mVertexData.PNT_VertexData);
	}

	if (mVertexData.bHasIndexData)
	{
		mIndexBuffer = std::make_shared<SSIndexBuffer>();
		mIndexBuffer->SetIndexBufferData(mVertexData.IndexData);
	}

	shared_ptr<SSVertexShader> vertexShader = SSShaderManager::Get().GetVertexShader(mRenderData.VertexShaderName);
	shared_ptr<SSPixelShader> pixelShader = SSShaderManager::Get().GetPixelShader(mRenderData.PixelShaderName);

	mMaterial = new SSMaterial(vertexShader, pixelShader);
}


void SSRenderTargetCube::Draw(ID3D11DeviceContext* deviceContext)
{
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

	
	// set vertex shader constants
	for (auto& [k, v] : mRenderData.VSConstantBufferMap)
	{
		mMaterial->SetVSConstantBufferProxyData(deviceContext, k, v);
	}

	// set pixel shader constants	
	for (auto& [k, v] : mRenderData.PSConstantBufferMap)
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
	if (mVertexData.bHasIndexData)
	{
		deviceContext->DrawIndexed(mIndexBuffer->GetIndexCount(), 0, 0);
	}
	else
	{
		deviceContext->Draw(mVertexBuffer->GetVertexCount(), 0);
	}


	mMaterial->ReleaseCurrent();
}

void SSRenderTargetCube::CreateVertexData()
{
	std::vector<VT_PositionNormalTexcoordTangent> vertexArray =
	{
		// front
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f,1), XMFLOAT3(0,0,-1), XMFLOAT2(0,0), XMFLOAT4(0,0,-1,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),  XMFLOAT2(0,1),XMFLOAT4(0,0,-1,1)},
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f,1), XMFLOAT3(0,0,-1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,-1,1) },
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f,1),  XMFLOAT3(0,0,-1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,-1,1) },

		// back
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,0) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(0,1) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,1) ,XMFLOAT4(0,0,1,1) },
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f,1),  XMFLOAT3(0,0,+1),   XMFLOAT2(1,0) ,XMFLOAT4(0,0,1,1) },

		// top
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,0),XMFLOAT4(0,1,0,1)},
		{ XMFLOAT4(1.0f, +1.0f, +1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,0),XMFLOAT4(0,1,0,1)},
		{ XMFLOAT4(1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(1,1),XMFLOAT4(0,1,0,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(0,1,0), XMFLOAT2(0,1),XMFLOAT4(0,1,0,1)},


		// bottom
		{ XMFLOAT4(-1.0f,-1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,0),XMFLOAT4(0,-1,0,1)},
		{ XMFLOAT4(1.0f, -1.0f, +1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,0),XMFLOAT4(0,-1,0,1)},
		{ XMFLOAT4(1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(1,1),XMFLOAT4(0,-1,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(0,-1,0), XMFLOAT2(0,1),XMFLOAT4(0,-1,0,1)},

		// left
		{ XMFLOAT4(-1.0f, +1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,0),XMFLOAT4(-1,0,0,1)},
		{ XMFLOAT4(-1.0f, +1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,0),XMFLOAT4(-1,0,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, -1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(1,1),XMFLOAT4(-1,0,0,1)},
		{ XMFLOAT4(-1.0f, -1.0f, +1.0f, 1), XMFLOAT3(-1,0,0), XMFLOAT2(0,1),XMFLOAT4(-1,0,0,1)},

		// right
		{ XMFLOAT4(+1.0f, +1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,0),XMFLOAT4(1,0,0,1)},
		{ XMFLOAT4(+1.0f, +1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,0),XMFLOAT4(1,0,0,1)},
		{ XMFLOAT4(+1.0f, -1.0f, +1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(1,1),XMFLOAT4(1,0,0,1)},
		{ XMFLOAT4(+1.0f, -1.0f, -1.0f, 1), XMFLOAT3(1,0,0), XMFLOAT2(0,1),XMFLOAT4(1,0,0,1)},
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

	mVertexData.IndexData = indexArray;
	mVertexData.VertexType = EVertexType::VT_PNTT;
	mVertexData.PrimitiveType = D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	mVertexData.bHasIndexData = true;
	mVertexData.PNTT_VertexData = std::move(vertexArray);	
}