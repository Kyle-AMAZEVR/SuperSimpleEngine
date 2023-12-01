#pragma once

#include "SSDX11PostProcess.h"
#include "FreqUsedConstantBufferTypes.h"

class SSDX11Device;

struct LightCBuffer
{
	XMFLOAT4 lightPosition[16];
	XMFLOAT4 lightColors[16];
	XMFLOAT4 lightMinMaxs[16];
};

class SSLightPostProcess : public SSDX11PostProcess
{
public:
	SSLightPostProcess(UINT width, UINT height);

	virtual void Draw(
		SSDX11Device* deviceContext,
		SSDX11Texture2D* input0,
		SSDX11Texture2D* input1,
		SSDX11Texture2D* input2,
		SSDX11Texture2D* input3,
		SSDX11Texture2D* input4,
		SSDX11Texture2D* input5) override;
		

	

protected:
	std::shared_ptr<class SSDX11VertexShader> mVertexShader;
	std::shared_ptr<class SSDX11PixelShader> mPixelShader;

	LightCBuffer mLightInfo;
};
