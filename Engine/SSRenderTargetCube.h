
#pragma once


#include "SSRenderingObject.h"

class ENGINE_API SSRenderTargetCube : public SSRenderingObject
{
public:
	SSRenderTargetCube();
	SSRenderTargetCube(SSName vs, SSName ps);
	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	
protected:
	void CreateVertexData();
};