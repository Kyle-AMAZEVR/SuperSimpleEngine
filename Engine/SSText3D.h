#pragma once

#include "SSSceneObject.h"

class ENGINE_API SSText3D : public SSSceneObject
{
public:

	SSText3D(std::string text);

	virtual void Draw(ID3D11DeviceContext* deviceContext, SSMaterial* material) override;

protected:


	void InternalCreate();

	std::string mTextContent;

	std::shared_ptr<class SSDX11VertexBuffer> mTextVB = nullptr;
	std::shared_ptr<class SSIndexBuffer> mTextIB = nullptr;
};