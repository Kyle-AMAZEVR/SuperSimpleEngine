#pragma once



class SSDX11RenderCommand
{
public:
	virtual void Execute() = 0;
};

class SSDX11ClearRenderTargetCommand
{
public:

	virtual void Execute() override;
};

class SSDX11SetVertexShaderCommand
{
public:
	virtual void Execute() override;
};