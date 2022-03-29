#pragma once


#include "SSRenderThreadObject.h"
#include <memory>

class SSRenderTargetCube : public SSRenderThreadObject
{
public:
	SSRenderTargetCube();
    virtual void Draw(ID3D11DeviceContext* deviceContext) override;
protected:
	std::shared_ptr<class SSDX11VertexBuffer> mVB;
	std::shared_ptr<class SSDX11IndexBuffer> mIB;
};

class SSCubeMapRenderingSphere : public SSRenderThreadObject
{
public:
    SSCubeMapRenderingSphere();
    virtual void Draw(ID3D11DeviceContext* deviceContext) override;
protected:
    std::shared_ptr<class SSDX11VertexBuffer> mVB;
    std::shared_ptr<class SSDX11IndexBuffer> mIB;
};