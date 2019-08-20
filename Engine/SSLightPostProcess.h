#pragma once

#include "SSPostProcess.h"
#include "FreqUsedConstantBufferTypes.h"

struct LightCBuffer
{
	XMFLOAT4 lightPosition[16];
	XMFLOAT4 lightColors[16];
	XMFLOAT4 lightMinMaxs[16];
};

class SSLightPostProcess : public SSPostProcess
{
public:
	SSLightPostProcess(UINT width, UINT height);

	virtual void Draw(SSTexture2DBase* input0,
		SSTexture2DBase* input1,
		SSTexture2DBase* input2,
		SSTexture2DBase* input3,
		SSTexture2DBase* input4,
		SSTexture2DBase* input5) override;
		

	

protected:
	std::shared_ptr<class SSVertexShader> mVertexShader;
	std::shared_ptr<class SSPixelShader> mPixelShader;

	LightCBuffer mLightInfo;
};
