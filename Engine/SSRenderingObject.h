#pragma once

#include "SSMeshRenderData.h"

class SSRenderingObject
{
public:
	SSRenderingObject(class SSObjectBase* pObject);
	
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
protected:
	SSObjectBase* mpObject = nullptr;

	SSMeshRenderData mRenderData;
	SSMeshVertexData mVertexData;

	class SSDX11VertexBuffer* mVertexBuffer = nullptr;
	class SSIndexBuffer* mIndexBuffer = nullptr;

	class SSMaterial* mMaterial = nullptr;
};