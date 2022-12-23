#pragma once

#include <memory>
#include "SSMeshRenderData.h"

class DX11RENDERER_API SSRenderingObject
{
public:
	SSRenderingObject(class SSObjectBase* pObject);
	SSRenderingObject(){}
	
	virtual ~SSRenderingObject();
	virtual void Draw(ID3D11DeviceContext* deviceContext);

	void SetPSTexture(SSName name, SSName textureName);
	void SetVSTexture(SSName name, SSName textureName);

	void SetVSConstantBufferData(SSName name, class SSConatantBufferData&& buffer);
	void SetPSConstantBufferData(SSName name, class SSConatantBufferData&& buffer);

	void SetVSConstantBufferData(SSName name, const SSConatantBufferData& buffer);
	void SetPSConstantBufferData(SSName name, const SSConatantBufferData& buffer);
		
	
protected:
	SSObjectBase* mpObject = nullptr;

	SSMeshRenderData mRenderData;
	SSMeshVertexData mVertexData;

	std::shared_ptr<class SSDX11VertexBuffer> mVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11InstancedVertexBuffer> mInstancedVertexBuffer = nullptr;
	std::shared_ptr<class SSDX11IndexBuffer> mIndexBuffer = nullptr;
	class SSMaterial* mMaterial = nullptr;
};