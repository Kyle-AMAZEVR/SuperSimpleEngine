#pragma once

#include <memory>
#include "SSMeshRenderData.h"

class SSRenderingObject
{
public:
	SSRenderingObject(class SSObjectBase* pObject);
	SSRenderingObject(){}
	
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);

protected:
	virtual void CreateRenderCmdList();
	std::vector<class SSRenderCmdBase*> RenderCmdList;

	SSObjectBase* mpObject = nullptr;

	SSMeshRenderData mRenderData;
	SSMeshVertexData mVertexData;

	std::shared_ptr<class SSDX11VertexBuffer> mVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11InstancedVertexBuffer> mInstancedVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11IndexBuffer> mIndexBuffer = nullptr;

	std::shared_ptr<class SSDX11VertexShader> mVS = nullptr;
	std::shared_ptr<class SSDX11PixelShader> mPS = nullptr;

	class SSMaterial* mMaterial = nullptr;
};