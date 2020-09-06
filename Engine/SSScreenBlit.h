#pragma once
#include "SSGameObject.h"


class ENGINE_API SSScreenBlit : public SSGameObject
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
	virtual void Destroy() override;

	protected:		
	std::shared_ptr<class SSDX11VertexBuffer> mScreenVB;
	std::shared_ptr<class SSIndexBuffer> mScreenIB;
};