#pragma once

#include "SSObjectBase.h"

class SSScreenBlit : public SSObjectBase
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);	

protected:		
	std::shared_ptr<class SSDX11VertexBuffer> mScreenVB;
	std::shared_ptr<class SSDX11IndexBuffer> mScreenIB;

	std::vector<class SSRenderCmdBase*> mCommands;
};