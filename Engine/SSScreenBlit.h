#pragma once

#include "SSRenderThreadObject.h"


class DX11RENDERER_API SSScreenBlit : public SSRenderThreadObject
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
	virtual void Destroy() override;

protected:		
	std::shared_ptr<class SSDX11VertexBuffer> mScreenVB;
	std::shared_ptr<class SSIndexBuffer> mScreenIB;
};