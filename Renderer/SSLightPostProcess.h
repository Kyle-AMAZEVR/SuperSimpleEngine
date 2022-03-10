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

	virtual void Draw(
		ID3D11DeviceContext* deviceContext,
		SSDX11Texture2D* input0,
		SSDX11Texture2D* input1,
		SSDX11Texture2D* input2,
		SSDX11Texture2D* input3,
		SSDX11Texture2D* input4,
		SSDX11Texture2D* input5) override;
		

	

protected:
	std::shared_ptr<class SSVertexShader> mVertexShader;
	std::shared_ptr<class SSPixelShader> mPixelShader;

	LightCBuffer mLightInfo;
};
