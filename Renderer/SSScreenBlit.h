#pragma once

#include "SSRenderThreadObject.h"


class SSScreenBlit : public SSRenderThreadObject
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
	virtual void Destroy() override;

protected:		
	std::shared_ptr<class SSDX11VertexBuffer> mScreenVB;
	std::shared_ptr<class SSDX11IndexBuffer> mScreenIB;

	std::vector<class SSRenderCmdBase*> mCommands;
};