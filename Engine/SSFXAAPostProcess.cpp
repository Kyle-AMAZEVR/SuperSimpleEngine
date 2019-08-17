

#include "Core.h"
#include "SSPostProcess.h"
#include "SSFXAAPostProcess.h"

#include "SSDrawCommand.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSShaderManager.h"
#include "SSScreenBlit.h"

SSFXAAPostProcess::SSFXAAPostProcess(UINT width, UINT height)
	: SSPostProcess(width, height, 1, false, false)
{
	mFXAAVertexShader = SSShaderManager::Get().GetVertexShader("FXAA.vs");
	mFXAAPixelShader = SSShaderManager::Get().GetPixelShader("FXAA.ps");
}

void SSFXAAPostProcess::Draw(SSTexture2DBase* input0)
{
	mRenderTarget->Clear();
	mRenderTarget->SetCurrentRenderTarget();

	SSDrawCommand fxaaDrawCmd{ mFXAAVertexShader.get(), mFXAAPixelShader.get(), mScreenBlit };
	fxaaDrawCmd.SetPSTexture("ScreenTex", input0);
	
	SSAlignedCBuffer<XMFLOAT2> invScreenSize;
	invScreenSize.value1.x = 1 / static_cast<float>(mWidth);
	invScreenSize.value1.y = 1 / static_cast<float>(mHeight);
	
	fxaaDrawCmd.StorePSConstantBufferData("CBInverseScreenSize", invScreenSize);

	fxaaDrawCmd.Do();
}
