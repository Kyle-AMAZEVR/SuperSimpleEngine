#pragma once

#include "SSDX11PostProcess.h"

class SSDX11Texture2D;
//
class DX11RENDERER_API SSFXAAPostProcess : public SSDX11PostProcess
{
public:
	SSFXAAPostProcess(UINT width, UINT height);

	virtual void Draw(class SSDX11Device* deviceContext, SSDX11Texture2D* input0) override;

protected:
	std::shared_ptr<class SSVertexShader> mFXAAVertexShader = nullptr;
	std::shared_ptr<class SSPixelShader> mFXAAPixelShader = nullptr;


};