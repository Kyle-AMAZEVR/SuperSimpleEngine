#pragma once
#include "SSPostProcess.h"

#include "SSShader.h"


enum class EDumpMode : unsigned char
{
	
};

class SSGBufferDumpPostProcess : public SSPostProcess
{
public:
	SSGBufferDumpPostProcess(UINT width, UINT height);
 
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2) override;

	

protected:
	std::shared_ptr<SSVertexShader> mVertexShader;
	std::shared_ptr<SSPixelShader> mPixelShader;
};
