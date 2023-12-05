#pragma once



class IRenderTarget
{
public:
	virtual unsigned int GetRenderTargetWidth() const = 0;
	virtual unsigned int GetRenderTargetHeight() const = 0;
	virtual void Resize(unsigned int width, unsigned int height) = 0;
};
