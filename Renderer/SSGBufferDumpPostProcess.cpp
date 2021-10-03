

#include "SSRendererModulePCH.h"
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

	UpdateDumpSettings();
}

void SSGBufferDumpPostProcess::Draw(ID3D11DeviceContext* deviceContext, SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2)
{
	check(deviceContext != nullptr);

	mRenderTarget->Clear(deviceContext);
	mRenderTarget->SetCurrentRenderTarget(deviceContext);

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

	gbufferDumpDrawCmd.Do(deviceContext);
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
