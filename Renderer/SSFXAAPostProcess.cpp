

#include "SSRendererModulePCH.h"
#include "SSDX11PostProcess.h"
#include "SSFXAAPostProcess.h"

#include "SSDrawCommand.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSShaderManager.h"
#include "SSScreenBlit.h"

SSFXAAPostProcess::SSFXAAPostProcess(UINT width, UINT height)
	: SSDX11PostProcess(width, height, 1, false, false)
{
	mFXAAVertexShader = SSShaderManager::Get().GetVertexShader("FXAA.vs");
	mFXAAPixelShader = SSShaderManager::Get().GetPixelShader("FXAA.ps");
}

void SSFXAAPostProcess::Draw(SSDX11Device* device, SSDX11Texture2D* input0)
{
	check(device != nullptr);

	mRenderTarget->Clear(device);
	mRenderTarget->SetCurrentRenderTarget(device);

	SSDrawCommand fxaaDrawCmd{ mFXAAVertexShader, mFXAAPixelShader, mScreenBlit };
	fxaaDrawCmd.SetPSTexture("ScreenTex", input0);
	
	SSAlignedCBuffer<XMFLOAT2> invScreenSize;
	invScreenSize.value1.x = 1 / static_cast<float>(mWidth);
	invScreenSize.value1.y = 1 / static_cast<float>(mHeight);
	
	fxaaDrawCmd.StorePSConstantBufferData("CBInverseScreenSize", invScreenSize);

	fxaaDrawCmd.Do(device);
}
