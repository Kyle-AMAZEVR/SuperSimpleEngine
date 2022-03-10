#pragma once


class RENDERCORE_API SSViewport
{
public:
	virtual void Resize(unsigned int width, unsigned int height) = 0;
	virtual void Clear() = 0;

protected:
	unsigned int mWidth;
	unsigned int mHeight;
	float ClearColor[4]{ 1,0,0,1 };
};