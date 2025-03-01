
#include "SSRendererModulePCH.h"
#include "SSDX11Renderer.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSDX11IndexBuffer.h"
#include "SSSamplerManager.h"
#include "SSDrawCommand.h"
#include "SSDX11GBuffer.h"
#include "SSScreenBlit.h"
#include "SSDX11RenderTarget.h"
#include "SSTextureCube.h"
#include "SSDepthStencilStateManager.h"
#include "SSRasterizeStateManager.h"
#include "SSCubemapRenderTarget.h"
#include "SSFreqUsedNames.h"
#include "SSFileHelper.h"
#include "SSShaderManager.h"
#include "SSMaterial.h"
#include "SSFXAAPostProcess.h"
#include "SSGBufferDumpPostProcess.h"
#include "SSLightPostProcess.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"
#include "SSSharedBufferCache.h"
#include "SSCustomRenderThreadObject.h"
#include "SSMathHelper.h"
#include "SSCameraManager.h"
#include "SSRenderCommand.h"
#include <iostream>

SSDX11Renderer* SSDX11Renderer::mRendererInstance = nullptr;
SSDX11Device* SSDX11Renderer::mDX11Device = nullptr;

SSDX11Renderer::SSDX11Renderer()	
{
	check(mRendererInstance == nullptr);
	mRendererInstance = this;
}

SSDX11Renderer& SSDX11Renderer::Get()
{
	return *mRendererInstance;
}

SSDX11Renderer* SSDX11Renderer::GetPtr()
{
	return mRendererInstance;
}

void SSDX11Renderer::SetWindowWidth(int inWidth)
{
	if (inWidth < 0)
	{
		mWindowWidth = 512;
	}
	else
	{
		mWindowWidth = inWidth;
	}
}
void SSDX11Renderer::SetWindowHeight(int inHeight)
{
	if (inHeight < 0)
	{
		mWindowHeight = 512;
	}
	else
	{
		mWindowHeight = inHeight;
	}
}

void SSDX11Renderer::Initialize(HWND windowHandle)
{
	mDX11Device = new SSDX11Device();
	mDX11Device->InitializeDevice(windowHandle);

	SSShaderManager::Get().Initialize();
	SSSamplerManager::Get().Initialize();
	SSDepthStencilStateManager::Get().Initialize();
	SSRasterizeStateManager::Get().Initialize();
	SSSharedBufferCache::Get().Initialize();
		
	mGBuffer = std::make_shared<SSDX11GBuffer>(mWindowWidth, mWindowHeight);

	mEquirectToCubemapRenderTarget = std::make_shared<SSCubemapRenderTarget>(1024, 1024);
	mConvolutionRenderTarget = std::make_shared<SSCubemapRenderTarget>(512, 512);
	mPrefilterRenderTarget = std::make_shared<SSPrefilterCubemapRenderTarget>(1024, 1024, 5);
	m2DLUTRenderTarget = std::make_shared<class SSDX11RenderTarget>(512, 512, 1, false);

	mFXAAPostProcess = std::make_shared<SSFXAAPostProcess>(mWindowWidth, mWindowHeight);
	mGBufferDumpProcess = std::make_shared<SSGBufferDumpPostProcess>(512, 512);
	mDeferredLightPostProcess = std::make_shared<SSLightPostProcess>(mWindowWidth, mWindowHeight);


	if (SSFileHelper::DirectoryExists(L"./Prebaked") == false)
	{
		SSFileHelper::MakeDirectory(L"./Prebaked");
	}	

	mScreenBlit = std::make_shared<class SSScreenBlit>();
	TestCompileShader();
	TestCreateResources();

	bInitialized = true;
		
	mDX11Device->ResizeRenderTarget(mWindowWidth, mWindowHeight);
}

void SSDX11Renderer::UpdateRenderingObjects()
{
	//
	SSRenderingObjectManager::Get().UpdateObjects();
}

void SSDX11Renderer::Shutdown()
{
	SSDepthStencilStateManager::Get().Shutdown();
	SSRasterizeStateManager::Get().Shutdown();
	SSSamplerManager::Get().Shutdown();
	SSShaderManager::Get().Shutdown();
	SSRenderingObjectManager::Get().Shutdown();
	SSSharedBufferCache::Get().Shutdown();

	mGBuffer.reset();
	mEquirectToCubemapRenderTarget.reset();
}

void SSDX11Renderer::TestCreateResources()
{
	auto* deviceContext = GetImmediateDeviceContext();

	mRenderTargetCube = std::make_shared<SSRenderTargetCube>();
	mTestCubeTexture = std::make_shared<SSTextureCube>();

	mCubemapSphere = std::make_shared<SSCubeMapRenderingSphere>();	

	mRenderTargetCube->SetScale(1,1,1);

	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");
}

void SSDX11Renderer::AppendRenderCommand(SSRenderCmdBase* cmd)
{
	mRenderCommandList.push_back(cmd);
}

void SSDX11Renderer::FlushRenderCommands()
{
	for(SSRenderCmdBase* cmd : mRenderCommandList)
	{
		cmd->Execute(mDX11Device->GetDeviceContext());
	}

	mRenderCommandList.clear();
}

void SSDX11Renderer::DrawDummyScene()
{
	if (!bInitialized)
	{
		return;
	}

	if (mPaused)
	{
		return;
	}
	
	mDX11Device->SetDefaultRenderTargetAsCurrent();
	float tmpColor[4]{ 1, 0, 0, 1 };
	mDX11Device->ClearDefaultRenderTargetView(tmpColor);
	
	mDX11Device->Present();
}

ID3D11Device* SSDX11Renderer::GetDevice()
{
	if(mDX11Device)
	{
		return mDX11Device->GetDevice();
	}
	return nullptr;
}

ID3D11DeviceContext* SSDX11Renderer::GetImmediateDeviceContext()
{
	if (mDX11Device)
	{
		return mDX11Device->GetDeviceContext();
	}

	return nullptr;
}

IDXGISwapChain* SSDX11Renderer::GetSwapChain()
{
	if(mDX11Device)
	{
		return mDX11Device->GetSwapChain();
	}

	return nullptr;
}

void SSDX11Renderer::DrawObject()
{
	if (bInitialized == false)
	{
		return;
	}

	if (mPaused)
	{
		return;
	}

	static bool bEquidirectToCubeDrawn = false;

	if (bEquidirectToCubeDrawn == false)
	{
		if (TryLoadEnvCubemap(L"./Prebaked/EnvCubemap.dds") == false)
		{
			CreateEnvCubemap();
		}
		bEquidirectToCubeDrawn = true;
	}

	mGBuffer->Clear(mDX11Device);
	mGBuffer->SetCurrentRenderTarget(mDX11Device);
	
	DrawSkybox();

	// draw game objects
	auto& objects = SSRenderingObjectManager::Get().GetRenderingObjectMap();
	for (auto [k, v] : objects)
	{
		v->Draw(mDX11Device->GetDeviceContext());
	}
	
	float ClearColor[4]{ 1,0,0,1 };
	mDX11Device->ClearDefaultRenderTargetView(ClearColor);
	mDX11Device->SetDefaultRenderTargetAsCurrent();

	SSDrawCommand blitDrawCmd{ mScreenBlitVertexShader, mScreenBlitPixelShader, mScreenBlit };
	
	blitDrawCmd.SetPSTexture("sampleTexture", mGBuffer->GetColorOutput());

	blitDrawCmd.Do(mDX11Device);

	mDX11Device->Present();
}

void SSDX11Renderer::DrawCubeScene()
{
	if (bInitialized == false)
	{
		return;
	}

	if (mPaused)
	{
		return;
	}

	static bool bEquidirectToCubeDrawn = false;
	static bool bConvolutionDrawn = false;
	static bool bPrefilterDrawn = false;
	static bool bLUTCreated = false;

	if (bEquidirectToCubeDrawn == false)
	{
		if (TryLoadEnvCubemap(L"./Prebaked/EnvCubemap.dds") == false)
		{
			CreateEnvCubemap();
		}
		bEquidirectToCubeDrawn = true;
	}

	if (bEquidirectToCubeDrawn == false)
	{
		if (TryLoadEnvCubemap(L"./Prebaked/EnvCubemap.dds") == false)
		{
			CreateEnvCubemap();
		}
		bEquidirectToCubeDrawn = true;
	}

	if (bConvolutionDrawn == false)
	{
		if (TryLoadEnvCubemapConvolution(L"./Prebaked/EnvConvolution.dds") == false)
		{
			CreateEnvCubemapConvolution();
		}
		bConvolutionDrawn = true;
	}
	if (bPrefilterDrawn == false)
	{
		if (TryLoadEnvCubemapPrefilter(L"./Prebaked/EnvPrefilter.dds") == false)
		{
			CreateEnvCubemapPrefilter();
		}
		bPrefilterDrawn = true;
	}

	if (bLUTCreated == false)
	{
		if (TryLoad2DLUTTexture() == false)
		{
			Create2DLUTTexture();
			m2DLUTTexture = SSDX11Texture2D::CreateFromDDSFile(L"./Prebaked/2DLUT.dds");
		}
		bLUTCreated = true;
	}

	// @draw cubemap to gbuffer
	// @start	

	mGBuffer->Clear(mDX11Device);
	mGBuffer->SetCurrentRenderTarget(mDX11Device);

	// cubemap draw
	DrawSkybox();

	// draw game objects
	auto& objects = SSRenderingObjectManager::Get().GetRenderingObjectMap();
	for (auto [k, v] : objects)
	{
		v->Draw(mDX11Device->GetDeviceContext());
	}	

	mDeferredLightPostProcess->Draw(
		mDX11Device,
		mGBuffer->GetPositionOutput(),
		mGBuffer->GetColorOutput(),
		mGBuffer->GetNormalOutput(),
		m2DLUTTexture.get(),
		mEnvCubemapConvolution.get(),
		mEnvCubemapPrefilter.get());

	mFXAAPostProcess->Draw(mDX11Device, mDeferredLightPostProcess->GetOutput(0));
	
	float ClearColor[4]{1,0,0,1};
	mDX11Device->ClearDefaultRenderTargetView(ClearColor);
	mDX11Device->SetDefaultRenderTargetAsCurrent();	
	
	SSDrawCommand blitDrawCmd{ mScreenBlitVertexShader, mScreenBlitPixelShader, mScreenBlit };
	
	if (false)
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mGBufferDumpProcess->GetOutput(0));
	}
	else
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mFXAAPostProcess->GetOutput(0));
	}

	blitDrawCmd.Do(mDX11Device);

	mDX11Device->Present();
}




void SSDX11Renderer::TestCompileShader()
{
	mScreenBlitVertexShader = SSShaderManager::Get().GetVertexShader("Screen.vs");
	mScreenBlitPixelShader = SSShaderManager::Get().GetPixelShader("Screen.ps");

	mDeferredVertexShader = SSShaderManager::Get().GetVertexShader("GBuffer.vs");
	mDeferredPixelShader = SSShaderManager::Get().GetPixelShader("GBuffer.ps");

	mTestMaterial = std::make_shared<SSMaterial>(mDeferredVertexShader, mDeferredPixelShader);

	mCubemapVertexShader = SSShaderManager::Get().GetVertexShader("CubemapShader.vs");
	mCubemapPixelShader = SSShaderManager::Get().GetPixelShader("CubemapShader.ps");

	mEquirectToCubemapVertexShader = SSShaderManager::Get().GetVertexShader("EquirectangleToCubemap.vs");
	mEquirectToCubemapPixelShader = SSShaderManager::Get().GetPixelShader("EquirectangleToCubemap.ps");

	mCubemapConvolutionVertexShader = SSShaderManager::Get().GetVertexShader("CubemapConvolution.vs");
	mCubemapConvolutionPixelShader = SSShaderManager::Get().GetPixelShader("CubemapConvolution.ps");

	mPrefilterVertexShader = SSShaderManager::Get().GetVertexShader("Prefilter.vs");
	mPrefilterPixelShader = SSShaderManager::Get().GetPixelShader("Prefilter.ps");

	mDeferredLightVertexShader = SSShaderManager::Get().GetVertexShader("DeferredLighting.vs");
	mDeferredLightPixelShader = SSShaderManager::Get().GetPixelShader("DeferredLighting.ps");

	m2DLUTVertexShader = SSShaderManager::Get().GetVertexShader("2DLUT.vs");
	m2DLUTPixelShader = SSShaderManager::Get().GetPixelShader("2DLUT.ps");

	mTBNDebugVertexShader = SSShaderManager::Get().GetVertexShader("TBNDebug.vs");
	mTBNDebugPixelShader = SSShaderManager::Get().GetPixelShader("TBNDebug.ps");

	mTBNDebugMaterial = std::make_shared<SSMaterial>(mTBNDebugVertexShader, mTBNDebugPixelShader);
	mDeferredLightMaterial = std::make_shared<SSMaterial>(mDeferredLightVertexShader, mDeferredLightPixelShader);

	//mRenderTargetCube = std::make_shared<SSRenderTargetCube>("Prefilter.vs", "Prefilter.ps");
}


void SSDX11Renderer::OnWindowResize(int newWidth, int newHeight)
{
	if (bInitialized)
	{	
		check(newWidth > 0 && newHeight > 0);

		this->Resize(newWidth, newHeight);
	}
}

void SSDX11Renderer::Resize(int newWidth,int newHeight)
{	
	mDX11Device->ResizeRenderTarget(newWidth, newHeight);
	mGBuffer->Resize(newWidth, newHeight);
	mFXAAPostProcess->OnResize(newWidth, newHeight);
	mGBufferDumpProcess->OnResize(newWidth, newHeight);
	mDeferredLightPostProcess->OnResize(newWidth, newHeight);
}


void SSDX11Renderer::Create2DLUTTexture()
{
	auto* deviceContext = GetImmediateDeviceContext();

	m2DLUTRenderTarget->Clear(mDX11Device);
	m2DLUTRenderTarget->SetCurrentRenderTarget(mDX11Device);

	SSDrawCommand blitDrawCmd{ m2DLUTVertexShader, m2DLUTPixelShader, mScreenBlit };
	blitDrawCmd.Do(mDX11Device);

	m2DLUTRenderTarget->SaveRTTexture(0, L"./Prebaked/2DLUT.dds");

}

void SSDX11Renderer::CreateEnvCubemapConvolution()
{
	auto* deviceContext = GetImmediateDeviceContext();

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);
	{
		mConvolutionRenderTarget->Clear(deviceContext);
		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_X);

        SSDrawCommand convolutionDrawCmd{ mCubemapConvolutionVertexShader, mCubemapConvolutionPixelShader, mRenderTargetCube };

		mConvolutionRenderTarget->Clear(deviceContext);
		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_X);

		convolutionDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
		convolutionDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		convolutionDrawCmd.SetPSTexture("EnvironmentMap", mEnvCubemap.get());

		SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

		convolutionDrawCmd.Do(mDX11Device);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		convolutionDrawCmd.Do(mDX11Device);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		convolutionDrawCmd.Do(mDX11Device);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		convolutionDrawCmd.Do(mDX11Device);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		convolutionDrawCmd.Do(mDX11Device);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		convolutionDrawCmd.Do(mDX11Device);

		SSRasterizeStateManager::Get().SetToDefault(deviceContext);

		mConvolutionRenderTarget->Copy2DRenderTargetToCubemap(deviceContext);

		mEnvCubemapConvolution = mConvolutionRenderTarget;

		mConvolutionRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvConvolution.dds");
	}
}

void SSDX11Renderer::CreateEnvCubemapPrefilter()
{
	auto* deviceContext = GetImmediateDeviceContext();

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);
	{
		SSDrawCommand prefilterDrawCmd{ mPrefilterVertexShader, mPrefilterPixelShader, mRenderTargetCube };

		mPrefilterRenderTarget->Clear(deviceContext);

		SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

		const int maxMipLevels = 5;


		for (UINT mip = 0; mip < 5; ++mip)
		{
			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_X, mip);

			prefilterDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
			prefilterDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
			CbufferFloat temp;
			temp.value = (float)mip / (float)(maxMipLevels - 1);


			prefilterDrawCmd.StorePSConstantBufferData(RoughnessName, temp);
			prefilterDrawCmd.SetPSTexture("EnvironmentMap", mEquirectToCubemapRenderTarget.get());

			prefilterDrawCmd.Do(mDX11Device);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
			prefilterDrawCmd.Do(mDX11Device);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
			prefilterDrawCmd.Do(mDX11Device);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
			prefilterDrawCmd.Do(mDX11Device);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
			prefilterDrawCmd.Do(mDX11Device);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
			prefilterDrawCmd.Do(mDX11Device);
		}
		SSRasterizeStateManager::Get().SetToDefault(GetImmediateDeviceContext());

		mPrefilterRenderTarget->Copy2DRenderTargetToCubemap(deviceContext);

		mEnvCubemapPrefilter = mPrefilterRenderTarget;

		mPrefilterRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvPrefilter.dds");
	}
}

bool SSDX11Renderer::TryLoadEnvCubemap(std::wstring filepath)
{
	mEnvCubemap = SSTextureCube::CreateFromDDSFile(filepath);
	return mEnvCubemap != nullptr;
}

bool SSDX11Renderer::TryLoadEnvCubemapPrefilter(std::wstring filepath)
{
	mEnvCubemapPrefilter = SSTextureCube::CreateFromDDSFile(filepath);
	return mEnvCubemapPrefilter != nullptr;
}

bool SSDX11Renderer::TryLoadEnvCubemapConvolution(std::wstring filepath)
{
	mEnvCubemapConvolution = SSTextureCube::CreateFromDDSFile(filepath);
	return mEnvCubemapConvolution != nullptr;
}


bool SSDX11Renderer::TryLoad2DLUTTexture()
{
	m2DLUTTexture = SSDX11Texture2D::CreateFromDDSFile(L"./Prebaked/2DLUT.dds");
	return m2DLUTTexture != nullptr;
}

void SSDX11Renderer::CreateEnvCubemap()
{
	auto* deviceContext = GetImmediateDeviceContext();

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);

	mHDREnvmap = SSDX11Texture2D::CreateFromHDRFile("./Resource/Tex/HDR/Circus_Backstage_3k.hdr");
	{
        SSDrawCommand equirectToCubeDrawCmd{ mEquirectToCubemapVertexShader, mEquirectToCubemapPixelShader, mRenderTargetCube };

		mEquirectToCubemapRenderTarget->Clear(deviceContext);
		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_X);

		equirectToCubeDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		equirectToCubeDrawCmd.SetPSTexture("sampleTexture", mHDREnvmap.get());

		SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

		equirectToCubeDrawCmd.Do(mDX11Device);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		equirectToCubeDrawCmd.Do(mDX11Device);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		equirectToCubeDrawCmd.Do(mDX11Device);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		equirectToCubeDrawCmd.Do(mDX11Device);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		equirectToCubeDrawCmd.Do(mDX11Device);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		equirectToCubeDrawCmd.Do(mDX11Device);

		SSRasterizeStateManager::Get().SetToDefault(deviceContext);

		mEquirectToCubemapRenderTarget->Copy2DRenderTargetToCubemap(deviceContext);

		mEquirectToCubemapRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvCubemap.dds");

		mEnvCubemap = mEquirectToCubemapRenderTarget;
	}
}



void SSDX11Renderer::DrawScene()
{	
	DrawCubeScene();
	//DrawObject();
}

void SSDX11Renderer::DrawSkybox()
{
    SSDrawCommand skyboxCmd{mCubemapVertexShader, mCubemapPixelShader, mCubemapSphere };

    XMMATRIX mvp = SSCameraManager::Get().GetCurrentCameraMVP();

    skyboxCmd.StoreVSConstantBufferData(MVPName, XMMatrixTranspose(mvp));

    skyboxCmd.SetPSTexture("gCubeMap", mEnvCubemap.get());

    auto* deviceContext = GetImmediateDeviceContext();

    SSDepthStencilStateManager::Get().SetDepthCompLessEqual(deviceContext);
    SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

    skyboxCmd.Do(mDX11Device);

    SSDepthStencilStateManager::Get().SetToDefault(deviceContext);
    SSRasterizeStateManager::Get().SetToDefault(deviceContext);
}

void SSDX11Renderer::CreateSkyBoxDrawCommands()
{
	AppendRenderCommand(new SSRenderCmdSetVS(mCubemapVertexShader)); // 

}


