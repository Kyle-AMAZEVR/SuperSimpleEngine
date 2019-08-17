#pragma once

#include "SSPostProcess.h"
//
class ENGINE_API SSFXAAPostProcess : public SSPostProcess
{
public:
	SSFXAAPostProcess(UINT width, UINT height);

	virtual void Draw(class SSTexture2DBase* input0) override;

protected:
	std::shared_ptr<class SSVertexShader> mFXAAVertexShader = nullptr;
	std::shared_ptr<class SSPixelShader> mFXAAPixelShader = nullptr;


};