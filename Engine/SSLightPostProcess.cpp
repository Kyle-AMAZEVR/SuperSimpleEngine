
#include "Core.h"
#include "SSLightPostProcess.h"
#include "SSShaderManager.h"
#include "SSDrawCommand.h"
#include "CameraManager.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSScreenBlit.h"





SSLightPostProcess::SSLightPostProcess(UINT width, UINT height)
	: SSPostProcess(width, height)
{
	mVertexShader = SSShaderManager::Get().GetVertexShader("DeferredLighting.vs");
	mPixelShader = SSShaderManager::Get().GetPixelShader("DeferredLighting.ps");

	for (int i = 0; i < 16; ++i)
	{
		mLightInfo.lightColors[i].x = mLightInfo.lightColors[i].y = mLightInfo.lightColors[i].z = 0.1;

		mLightInfo.lightPosition[i].x = i * 10;
		mLightInfo.lightPosition[i].y = 5;
		mLightInfo.lightPosition[i].z = 0;
		mLightInfo.lightPosition[i].z = 1;

		mLightInfo.lightMinMaxs[i].x = 1;
		mLightInfo.lightMinMaxs[i].y = 10;
		mLightInfo.lightMinMaxs[i].z = 0;
		mLightInfo.lightMinMaxs[i].w = 0;
	}
}



void SSLightPostProcess::Draw(SSTexture2DBase* input0, 
	SSTexture2DBase* input1, SSTexture2DBase* input2,
	SSTexture2DBase* input3, SSTexture2DBase* input4, SSTexture2DBase* input5)
{
	mRenderTarget->Clear();
	mRenderTarget->SetCurrentRenderTarget();
		
	SSDrawCommand drawCmd{mVertexShader.get(), mPixelShader.get(), mScreenBlit };

	SSAlignedCBuffer<XMMATRIX, XMMATRIX> cbuffer;
	cbuffer.value1 = XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView());
	cbuffer.value2 = XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj());

	drawCmd.StorePSConstantBufferData("CameraTransform", cbuffer);
	drawCmd.SetPSTexture("PositionTex", input0);
	drawCmd.SetPSTexture("DiffuseTex", input1);
	drawCmd.SetPSTexture("NormalTex", input2);
	drawCmd.SetPSTexture("BrdfLUT", input3);
	drawCmd.SetPSTexture("IrradianceMap", input4);
	drawCmd.SetPSTexture("PrefilterMap", input5);

	drawCmd.StorePSConstantBufferData("LightPosition", mLightInfo.lightPosition);
	drawCmd.StorePSConstantBufferData("LightColor", mLightInfo.lightColors);
	drawCmd.StorePSConstantBufferData("LightMinMax", mLightInfo.lightMinMaxs);
	drawCmd.Do();
}