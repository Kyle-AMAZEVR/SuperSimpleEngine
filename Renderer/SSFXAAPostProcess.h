#pragma once

#include "SSPostProcess.h"

class SSDX11Texture2D;
//
class DX11RENDERER_API SSFXAAPostProcess : public SSPostProcess
{
public:
	SSFXAAPostProcess(UINT width, UINT height);

	virtual void Draw(ID3D11DeviceContext* deviceContext, SSDX11Texture2D* input0) override;

protected:
	std::shared_ptr<class SSVertexShader> mFXAAVertexShader = nullptr;
	std::shared_ptr<class SSPixelShader> mFXAAPixelShader = nullptr;


};