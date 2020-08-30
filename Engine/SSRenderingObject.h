#pragma once

#include "SSMeshRenderData.h"

class ENGINE_API SSRenderingObject
{
public:
	SSRenderingObject(class SSObjectBase* pObject);
	SSRenderingObject(){}
	
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);

	void SetPSTexture(SSName name, SSName textureName);
	void SetVSTexture(SSName name, SSName textureName);

	void SetVSConstantBufferData(SSName name, class SSConstantBufferProxy&& buffer);
	void SetPSConstantBufferData(SSName name, class SSConstantBufferProxy&& buffer);

	void SetVSConstantBufferData(SSName name, const SSConstantBufferProxy& buffer);
	void SetPSConstantBufferData(SSName name, const SSConstantBufferProxy& buffer);
		
	
protected:
	SSObjectBase* mpObject = nullptr;

	SSMeshRenderData mRenderData;
	SSMeshVertexData mVertexData;

	class SSDX11VertexBuffer* mVertexBuffer = nullptr;
	class SSIndexBuffer* mIndexBuffer = nullptr;
	class SSMaterial* mMaterial = nullptr;
};