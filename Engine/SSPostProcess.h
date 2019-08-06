#pragma once


class ENGINE_API SSPostProcess
{
public:	
	SSPostProcess(UINT);
	virtual ~SSPostProcess();

	virtual void Draw(SSTexture2DBase* input0) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1) {}

protected:
	class SSRenderTargetTexture2D* mOutput;
};