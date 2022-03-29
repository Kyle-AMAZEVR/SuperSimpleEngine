

#include "SSRendererModulePCH.h"
#include "SSDX11PostProcess.h"
#include "SSDrawCommand.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSShaderManager.h"
#include "SSScreenBlit.h"
#include "SSGBufferDumpPostProcess.h"

SSGBufferDumpPostProcess::SSGBufferDumpPostProcess(UINT width, UINT height)
	: SSDX11PostProcess(width, height)
{
	mVertexShader = SSShaderManager::Get().GetVertexShader("GBufferDump.vs");
	mPixelShader = SSShaderManager::Get().GetPixelShader("GBufferDump.ps");

	UpdateDumpSettings();
}

void SSGBufferDumpPostProcess::Draw(SSDX11Device* device, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2)
{
	check(device != nullptr);

	mRenderTarget->Clear(device);
	mRenderTarget->SetCurrentRenderTarget(device);

	SSAlignedCBuffer<int, int, int, int,int> mDumpSettings;

	mDumpMode = EDumpMode::EDiffuseDump;

	mDumpSettings.value1 = (mDumpMode == EDumpMode::EPositionDump);
	mDumpSettings.value2 = (mDumpMode == EDumpMode::ENormalDump);
	mDumpSettings.value3 = (mDumpMode == EDumpMode::EMetalicDump);
	mDumpSettings.value4 = (mDumpMode == EDumpMode::EDiffuseDump);
	mDumpSettings.value5 = (mDumpMode == EDumpMode::ERoughnessDump);	

	SSDrawCommand gbufferDumpDrawCmd{ mVertexShader, mPixelShader, mScreenBlit };
	gbufferDumpDrawCmd.SetPSTexture("PositionTex", input0);
	gbufferDumpDrawCmd.SetPSTexture("DiffuseTex", input1);
	gbufferDumpDrawCmd.SetPSTexture("NormalTex", input2);

	gbufferDumpDrawCmd.StorePSConstantBufferData("Dump", mDumpSettings);

	gbufferDumpDrawCmd.Do(device);
}

void SSGBufferDumpPostProcess::ChangeNextDumpMode()
{
	switch(mDumpMode)
	{
	case EDumpMode::EPositionDump:
		mDumpMode = EDumpMode::ENormalDump;
		break;
	case EDumpMode::ENormalDump:
		mDumpMode = EDumpMode::EMetalicDump;
		break;
	case EDumpMode::EMetalicDump:
		mDumpMode = EDumpMode::EDiffuseDump;
		break;
	case EDumpMode::EDiffuseDump:
		mDumpMode = EDumpMode::ERoughnessDump;
		break;
	case EDumpMode::ERoughnessDump:
		mDumpMode = EDumpMode::EPositionDump;
		break;
	}

	UpdateDumpSettings();
}
 

void SSGBufferDumpPostProcess::UpdateDumpSettings()
{
	//mDumpSettings.value1 = (mDumpMode == EDumpMode::EPositionDump);
	//mDumpSettings.value2 = (mDumpMode == EDumpMode::ENormalDump);
	//mDumpSettings.value3 = (mDumpMode == EDumpMode::EMetalicDump);
	//mDumpSettings.value4 = (mDumpMode == EDumpMode::EDiffuseDump);
	//mDumpSettings.value5 = (mDumpMode == EDumpMode::ERoughnessDump);
}
