#pragma once

#include "SSDX11PostProcess.h"

class SSDX11Texture2D;
//
class SSFXAAPostProcess : public SSDX11PostProcess
{
public:
	SSFXAAPostProcess(UINT width, UINT height);

	virtual void Draw(class SSDX11Device* deviceContext, SSDX11Texture2D* input0) override;

protected:
	std::shared_ptr<class SSDX11VertexShader> mFXAAVertexShader = nullptr;
	std::shared_ptr<class SSDX11PixelShader> mFXAAPixelShader = nullptr;


};