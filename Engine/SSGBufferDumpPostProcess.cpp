

#include "Core.h"
#include "SSPostProcess.h"
#include "SSDrawCommand.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSShaderManager.h"
#include "SSScreenBlit.h"
#include "SSGBufferDumpPostProcess.h"

SSGBufferDumpPostProcess::SSGBufferDumpPostProcess(UINT width, UINT height)
	: SSPostProcess(width, height)
{
	mVertexShader = SSShaderManager::Get().GetVertexShader("GBufferDump.vs");
	mPixelShader = SSShaderManager::Get().GetPixelShader("GBufferDump.ps");
}

void SSGBufferDumpPostProcess::Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2)
{
	mRenderTarget->Clear();
	mRenderTarget->SetCurrentRenderTarget();

	SSDrawCommand gbufferDumpDrawCmd{ mVertexShader.get(), mPixelShader.get(), mScreenBlit };
	gbufferDumpDrawCmd.SetPSTexture("PositionTex", input0);
	gbufferDumpDrawCmd.SetPSTexture("DiffuseTex", input1);
	gbufferDumpDrawCmd.SetPSTexture("NormalTex", input2);	

	gbufferDumpDrawCmd.Do();
}