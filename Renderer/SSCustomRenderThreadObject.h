#pragma once

#include "SSObjectBase.h"
#include <memory>

class SSRenderTargetCube : public SSObjectBase
{
public:
	SSRenderTargetCube();
    virtual void Draw(ID3D11DeviceContext* deviceContext) override;
protected:
	std::shared_ptr<class SSDX11VertexBuffer> mVB;
	std::shared_ptr<class SSDX11IndexBuffer> mIB;
};

class SSCubeMapRenderingSphere : public SSObjectBase
{
public:
    SSCubeMapRenderingSphere();
    virtual void Draw(ID3D11DeviceContext* deviceContext) override;
protected:
    std::shared_ptr<class SSDX11VertexBuffer> mVB;
    std::shared_ptr<class SSDX11IndexBuffer> mIB;
};