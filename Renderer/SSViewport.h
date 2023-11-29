#pragma once


class DX11RENDERER_API SSViewport
{
public:
	virtual void Resize(class SSRenderDevice* device, unsigned int width, unsigned int height) = 0;
	virtual void Clear(SSRenderDevice* device) = 0;
	virtual void SetCurrentRenderTarget(SSRenderDevice* device) = 0;
protected:
	unsigned int mWidth;
	unsigned int mHeight;
	float ClearColor[4]{ 1,0,0,1 };
};