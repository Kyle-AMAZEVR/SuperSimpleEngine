
#include "SSRendererModulePCH.h"
#include "SSLightPostProcess.h"
#include "SSShaderManager.h"
#include "SSDrawCommand.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSScreenBlit.h"
#include "SSCameraManager.h"

SSLightPostProcess::SSLightPostProcess(UINT width, UINT height)
	: SSPostProcess(width, height)
{
	mVertexShader = SSShaderManager::Get().GetVertexShader("DeferredLighting.vs");
	mPixelShader = SSShaderManager::Get().GetPixelShader("DeferredLighting.ps");

	for (int i = 0; i < 16; ++i)
	{
		mLightInfo.lightColors[i].x = mLightInfo.lightColors[i].y = mLightInfo.lightColors[i].z = 0.01f;

		mLightInfo.lightPosition[i].x = i * 30.f;
		mLightInfo.lightPosition[i].y = 20;
		mLightInfo.lightPosition[i].z = 0;
		mLightInfo.lightPosition[i].w = 1;

		mLightInfo.lightMinMaxs[i].x = 1;
		mLightInfo.lightMinMaxs[i].y = 10;
		mLightInfo.lightMinMaxs[i].z = 0;
		mLightInfo.lightMinMaxs[i].w = 0;
	}
}



void SSLightPostProcess::Draw(
	ID3D11DeviceContext* deviceContext,
	SSDX11Texture2D* input0, 
	SSDX11Texture2D* input1, SSDX11Texture2D* input2,
	SSDX11Texture2D* input3, SSDX11Texture2D* input4, SSDX11Texture2D* input5)
{
	check(deviceContext != nullptr);

	mRenderTarget->Clear(deviceContext);
	mRenderTarget->SetCurrentRenderTarget(deviceContext);
		
	SSDrawCommand drawCmd{mVertexShader, mPixelShader, mScreenBlit };

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
	drawCmd.Do(deviceContext);
}