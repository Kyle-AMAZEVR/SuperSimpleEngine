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
	virtual void Draw(std::vector<class SSRenderCmdBase*>& inCmdList);
	
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