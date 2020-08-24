#pragma once

#include "SSMeshRenderData.h"

class SSRenderingObject
{
public:
	SSRenderingObject(class SSGameObject* pGameObject);
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
protected:
	SSGameObject* mpGameObject = nullptr;

	SSMeshRenderData mRenderData;

	class SSDX11VertexBuffer* mVertexBuffer = nullptr;
	class SSIndexBuffer* mIndexBuffer = nullptr;
};