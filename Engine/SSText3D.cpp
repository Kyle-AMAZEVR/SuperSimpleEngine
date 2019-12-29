
#include "Core.h"
#include "SSText3D.h"
#include "SSFontManager.h"
#include "SSMaterial.h"
#include "SSMathHelper.h"
#include "SSFreqUsedNames.h"
#include "SSName.h"
#include "CameraManager.h"
#include "SSVertexBuffer.h"
#include "SSIndexBuffer.h"

SSText3D::SSText3D(std::string text)
{
	check(text.size() > 0);
	mTextContent = text;
	InternalCreate();
}

void SSText3D::InternalCreate()
{
	std::vector<VT_PositionColor> vertices;
	std::vector<UINT> indices;

	SSFontCharacter& charInfo = SSFontManager::Get().GetCharacterInfo(mTextContent[0]);

	for(int i = 0; i < charInfo.mPoints.size(); ++i)
	{
		VT_PositionColor v;
		v.VertexAttribute1 = XMFLOAT4(charInfo.mPoints[i].x, charInfo.mPoints[i].y, 0 , 1.0f);		
		v.VertexAttribute2 = SSMathHelper::UnitX3;
		vertices.push_back(v);
		indices.push_back(static_cast<UINT>(vertices.size() - 1));
	}

	mTextVB = std::make_shared<SSVertexBuffer>();
	mTextIB = std::make_shared<SSIndexBuffer>();

	mTextVB->SetVertexBufferData(vertices);
	mTextIB->SetIndexBufferData(indices);
}


void SSText3D::Draw(ID3D11DeviceContext* deviceContext, SSMaterial* material)
{
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP_ADJ);
	
	auto stride = mTextVB->GetStride();
	UINT offset = 0;

	deviceContext->IASetVertexBuffers(0, 1, mTextVB->GetBufferPointerRef(), &stride, &offset);
	deviceContext->IASetIndexBuffer(mTextIB->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

	material->SetCurrent();

	material->SetVSConstantBufferData(deviceContext, ModelName, XMMatrixTranspose(XMMatrixScaling(1.0, 1.0, 1.0)));
	material->SetVSConstantBufferData(deviceContext, ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	material->SetVSConstantBufferData(deviceContext, ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));

	deviceContext->Draw(mTextVB->GetVertexCount(), 0);
}