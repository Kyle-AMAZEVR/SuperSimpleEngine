#pragma once


class DX11RENDERER_API SSViewport
{
public:
	virtual void Resize(class SSDX11Device* device, unsigned int width, unsigned int height) = 0;
	virtual void Clear(SSDX11Device* device) = 0;
	virtual void SetCurrentRenderTarget(SSDX11Device* device) = 0;
protected:
	unsigned int mWidth;
	unsigned int mHeight;
	float ClearColor[4]{ 1,0,0,1 };
};