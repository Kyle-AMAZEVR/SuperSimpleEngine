#pragma once

#include "SSGameObject.h"

class DX11RENDERER_API SSText3D : public SSGameObject
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