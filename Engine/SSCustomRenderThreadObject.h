#pragma once


#include "SSRenderThreadObject.h"
#include <memory>

class SSRenderTargetCube : public SSRenderThreadObject
{
public:
	SSRenderTargetCube();
	
protected:
	std::shared_ptr<class SSDX11VertexBuffer> mVB;
	std::shared_ptr<class SSIndexBuffer> mIB;
};