
#include "Core.h"
#include "SSDX11Renderer.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSCameraManager.h"
#include "SSDX11VertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSTexture2D.h"
#include "SSCube.h"
#include "SSSamplerManager.h"
#include "SSDrawCommand.h"
#include "SSGBuffer.h"
#include "SSScreenBlit.h"
#include "SSRenderTarget2D.h"
#include "SSTextureCube.h"
#include "SSDepthStencilStateManager.h"
#include "SSRasterizeStateManager.h"
#include "SSCubemapRenderTarget.h"
#include "SSMathHelper.h"
#include "SSFreqUsedNames.h"
#include "SSFileHelper.h"
#include "SSObjMesh.h"
#include "SSShaderManager.h"
#include "SSMaterial.h"
#include "SSFXAAPostProcess.h"
#include "SSGBufferDumpPostProcess.h"
#include "SSLightPostProcess.h"
#include "SSSphere.h"
#include "SSGameWindow.h"
#include "SSRenderingObjectManager.h"
#include "SSRenderingObject.h"
#include "SSSharedBufferCache.h"
#include "SSCustomRenderThreadObject.h"
#include <iostream>

SSDX11Renderer::SSDX11Renderer()	
{

}


void SSDX11Renderer::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;
	IDXGIFactory* pFactory = nullptr;
	CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory);
	if (pFactory)
	{
		IDXGIAdapter* pAdapter = nullptr;
		HRESULT AdapterResult = S_FALSE;
		UINT i = 0;
		do
		{
			AdapterResult = pFactory->EnumAdapters(i, &pAdapter);
			if (AdapterResult == S_OK)
			{
				DXGI_ADAPTER_DESC desc;
				pAdapter->GetDesc(&desc);
				
				mAdapterInfos.push_back(SSAdapterInfo{ desc, pAdapter });
			}
			i++;
		} while (AdapterResult == S_OK);
		
	}
	CreateDevice();
	CreateSwapChain();	

	SSShaderManager::Get().Initialize();
	SSSamplerManager::Get().Initialize();
	SSDepthStencilStateManager::Get().Initialize();
	SSRasterizeStateManager::Get().Initialize();
	SSSharedBufferCache::Get().Initialize();

	mViewport = std::make_shared<SSViewport>();
	mGBuffer = std::make_shared<SSGBuffer>(1024, 768);

	mEquirectToCubemapRenderTarget = std::make_shared<SSCubemapRenderTarget>(1024, 1024);
	mConvolutionRenderTarget = std::make_shared<SSCubemapRenderTarget>(512, 512);
	mPrefilterRenderTarget = std::make_shared<SSPrefilterCubemapRenderTarget>(1024, 1024, 5);
	m2DLUTRenderTarget = std::make_shared<class SSGenericRenderTarget>(512, 512, 1, false);

	mFXAAPostProcess = std::make_shared<SSFXAAPostProcess>(1024, 768);
	mGBufferDumpProcess = std::make_shared<SSGBufferDumpPostProcess>(512, 512);
	mDeferredLightPostProcess = std::make_shared<SSLightPostProcess>(1024, 768);


	if (SSFileHelper::DirectoryExists(L"./Prebaked") == false)
	{
		SSFileHelper::MakeDirectory(L"./Prebaked");
	}	



	mScreenBlit = std::make_shared<class SSScreenBlit>();
	TestCompileShader();
	TestCreateResources();

	bInitialized = true;

	Resize(mBufferWidth, mBufferHeight);
}


void SSDX11Renderer::Shutdown()
{
	SSDepthStencilStateManager::Get().Shutdown();
	SSRasterizeStateManager::Get().Shutdown();
	SSSamplerManager::Get().Shutdown();
	SSShaderManager::Get().Shutdown();

	mSwapChain.Reset();
	mDevice.Reset();
	mDeviceContext.Reset();
}

void SSDX11Renderer::TestCreateResources()
{
	auto* deviceContext = GetImmediateDeviceContext();

	mRenderTargetCube = std::make_shared<SSRenderTargetCube>();
	mTestCubeTexture = std::make_shared<SSTextureCube>();

	mCubemapSphere = std::make_shared<SSCubeMapRenderingSphere>();
	/*mTestSphere2 = std::make_shared<SSSphere>(25, 25, 10.0f);
	mTestSphere2->SetPosition(0, 20, 20);
	mTestSphere2->SetMetalicValue(0.f);
	mTestSphere2->SetRoughnessValue(1.f);

	mRustedIron = std::make_shared<SSPBRSphere>(
		deviceContext,
		"./Resource/Tex/rustediron/rustediron2_basecolor.dds",
		"./Resource/Tex/rustediron/rustediron2_normal.dds",
		"./Resource/Tex/rustediron/rustediron2_metallic.dds",
		"./Resource/Tex/rustediron/rustediron2_roughness.dds"
		);

	mTile = std::make_shared<SSPBRSphere>(
		deviceContext,
		"./Resource/Tex/tile/Tiles32_col.dds",
		"./Resource/Tex/tile/Tiles32_nrm.dds",
		"./Resource/Tex/tile/Tiles32_disp.dds",
		"./Resource/Tex/tile/Tiles32_rgh.dds"
		);

	mMetalGrid = std::make_shared<SSPBRSphere>(
		deviceContext,
		"./Resource/Tex/metalgrid/metalgrid4_basecolor.dds",
		"./Resource/Tex/metalgrid/metalgrid4_normal-dx.dds",
		"./Resource/Tex/metalgrid/metalgrid4_metallic.dds",
		"./Resource/Tex/metalgrid/metalgrid4_roughness.dds"
		);


	mRustedIron->SetPosition(0, 20, 0);
	mTile->SetPosition(20, 20, 20);
	mMetalGrid->SetPosition(40, 20, 40);
	*/

	mSponzaMesh = std::make_shared<SSObjMesh>();


	mRenderTargetCube->SetScale(1,1,1);

	if (SSFileHelper::FileExists(L"./Prebaked/sponza.mesh"))		
	{
		mSponzaMesh->LoadCookedFile("./Prebaked/sponza.mesh");
	}
	else
	{
		mSponzaMesh->ImportObjFile("./Resource/ObjMesh/sponza2.obj", "./Resource/ObjMesh/sponza2.mtl");
	}	
	

	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");
}

void SSDX11Renderer::AppendRenderCommand(SSDrawCmdBase* cmd)
{
	mRenderCommandList.push_back(cmd);
}

void SSDX11Renderer::FlushRenderCommands()
{
	for(SSDrawCmdBase* cmd : mRenderCommandList)
	{
		cmd->Do(mDeviceContext.Get());
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
	auto* deviceContext = GetImmediateDeviceContext();
	check(deviceContext);

	mViewport->SetCurrentRenderTarget(deviceContext);
	mViewport->Clear(deviceContext);


	mSwapChain->Present(0, 0);
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


	// @equirect to cube
	// @start
	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);

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
		}
		bLUTCreated = true;
	}

	// @draw cubemap to gbuffer
	// @start

	ID3D11DeviceContext* deviceContext = mDeviceContext.Get();

	mGBuffer->Clear(mDeviceContext.Get());
	mGBuffer->SetCurrentRenderTarget(mDeviceContext.Get());

	// cubemap draw
	DrawSkybox();

	// draw game objects
	auto& objects = SSRenderingObjectManager::Get().GetRenderingObjectMap();
	for (auto [k, v] : objects)
	{
		v->Draw(deviceContext);
	}

	mDeferredLightPostProcess->Draw(
		deviceContext,
		mGBuffer->GetPositionOutput(),
		mGBuffer->GetColorOutput(),
		mGBuffer->GetNormalOutput(),
		m2DLUTTexture.get(),
		mEnvCubemapConvolution.get(),
		mEnvCubemapPrefilter.get());

	mFXAAPostProcess->Draw(deviceContext, mDeferredLightPostProcess->GetOutput(0));

	mViewport->Clear(deviceContext);
	mViewport->SetCurrentRenderTarget(deviceContext);

	SSDrawCommand blitDrawCmd{ mScreenBlitVertexShader, mScreenBlitPixelShader, mScreenBlit };

	if (false)
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mGBufferDumpProcess->GetOutput(0));
	}
	else
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mFXAAPostProcess->GetOutput(0));
	}

	blitDrawCmd.Do(deviceContext);


	HRESULT presentResult = mSwapChain->Present(0, 0);

	if (presentResult == DXGI_ERROR_DEVICE_HUNG ||
		presentResult == DXGI_ERROR_DEVICE_REMOVED ||
		presentResult == DXGI_ERROR_DEVICE_RESET)
	{
		HRESULT removedReason = mDevice->GetDeviceRemovedReason();

		wchar_t outString[100];
		size_t size = 100;
		swprintf_s(outString, size, L"Device removed! DXGI_ERROR code: 0x%X\n", removedReason);
		OutputDebugStringW(outString);
	}
}



void SSDX11Renderer::DrawSponzaScene()
{
	if (bInitialized == false)
	{
		return;
	}

	if (mPaused)
	{
		return;
	}

	check(mDeviceContext != nullptr);

	// @equirect to cube
	// @start
	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);

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
		}
		bLUTCreated = true;
	}

	// @end

	auto* deviceContext = GetImmediateDeviceContext();
	check(deviceContext);
	SSRasterizeStateManager::Get().SetToDefault(deviceContext);

	// @draw cubemap to gbuffer
	// @start
	mGBuffer->Clear(deviceContext);
	mGBuffer->SetCurrentRenderTarget(deviceContext);

	DrawSkybox();

	mSponzaMesh->Draw(deviceContext, mTestMaterial.get());

	mGBufferDumpProcess->Draw(deviceContext, mGBuffer->GetPositionOutput(), mGBuffer->GetColorOutput(), mGBuffer->GetNormalOutput());

	mDeferredLightPostProcess->Draw(
		deviceContext,
		mGBuffer->GetPositionOutput(),
		mGBuffer->GetColorOutput(),
		mGBuffer->GetNormalOutput(),
		m2DLUTTexture.get(),
		mEnvCubemapConvolution.get(),
		mEnvCubemapPrefilter.get());

    mFXAAPostProcess->Draw(deviceContext, mDeferredLightPostProcess->GetOutput(0));

	mViewport->Clear(deviceContext);
	mViewport->SetCurrentRenderTarget(deviceContext);

	SSDrawCommand blitDrawCmd{ mScreenBlitVertexShader, mScreenBlitPixelShader, mScreenBlit };

	if (bGbufferDump)
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mGBufferDumpProcess->GetOutput(0));
	}
	else
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mFXAAPostProcess->GetOutput(0));
	}

	blitDrawCmd.Do(deviceContext);

	HRESULT presentResult = mSwapChain->Present(0, 0);
	
	if (presentResult == DXGI_ERROR_DEVICE_HUNG ||
		presentResult == DXGI_ERROR_DEVICE_REMOVED ||
		presentResult == DXGI_ERROR_DEVICE_RESET)
	{
		OutputDebugStringA("DXGI_Error");
	}

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

bool SSDX11Renderer::CreateDevice()
{
	//
	D3D_FEATURE_LEVEL featureLevelArray[] =
	{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
		
	};

	auto length = sizeof(featureLevelArray) / sizeof(D3D_FEATURE_LEVEL);

	D3D_FEATURE_LEVEL outFeatureLevel;

	HRESULT hr = D3D11CreateDevice(mAdapterInfos[0].AdapterPointer,
		D3D_DRIVER_TYPE_UNKNOWN, 
		0, 
		D3D11_CREATE_DEVICE_DEBUG, 
		featureLevelArray,
		length, 
		D3D11_SDK_VERSION, mDevice.GetAddressOf(), &outFeatureLevel, mDeviceContext.ReleaseAndGetAddressOf());


	return true;
}

void SSDX11Renderer::OnWindowResize(int newWidth, int newHeight)
{
	if (bInitialized)
	{	
		check(mDeviceContext);
		check(mDevice);
		check(mSwapChain);

		check(newWidth > 0 && newHeight > 0);

		if (mBufferWidth != newWidth || mBufferHeight != newHeight)
		{
			Resize(newWidth, newHeight);
		}
	}
}

void SSDX11Renderer::Resize(int newWidth,int newHeight)
{
	mBufferWidth = newWidth;
	mBufferHeight = newHeight;

	mViewport->Resize(newWidth, newHeight);
	mGBuffer->Resize(newWidth, newHeight);

	mFXAAPostProcess->OnResize(newWidth, newHeight);
	mGBufferDumpProcess->OnResize(newWidth, newHeight);
	mDeferredLightPostProcess->OnResize(newWidth, newHeight);
}


bool SSDX11Renderer::CreateSwapChain()
{
	HR(mDevice->CheckMultisampleQualityLevels(SwapChainFormat, 4, &m4xMSAAQuality));

	HR(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebug));

	IDXGIDevice* dxgiDevice = nullptr;
	HR(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdaptor = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdaptor));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	// Create swap chain
	IDXGIFactory2* dxgiFactory2 = nullptr;
	HR(dxgiFactory->QueryInterface(__uuidof(IDXGIFactory2), reinterpret_cast<void**>(&dxgiFactory2)));
	if (dxgiFactory2)
	{
		// DirectX 11.1 or later
		ID3D11Device1* Device1;
		ID3D11DeviceContext1* DeviceContext1;
		IDXGISwapChain1* SwapChain1;
		HR(mDevice->QueryInterface(__uuidof(ID3D11Device1), reinterpret_cast<void**>(&Device1)));
		{
			(void)mDevice->QueryInterface(__uuidof(ID3D11DeviceContext1), reinterpret_cast<void**>(&DeviceContext1));
		}

		DXGI_SWAP_CHAIN_DESC1 sd = {};
		sd.Width = mBufferWidth;
		sd.Height = mBufferHeight;
		sd.Format = SwapChainFormat;
		sd.SampleDesc.Count = 4;
		sd.SampleDesc.Quality = m4xMSAAQuality-1;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 2;
		sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

		HR(dxgiFactory2->CreateSwapChainForHwnd(mDevice.Get(), mWindowHandle, &sd, nullptr, nullptr, &SwapChain1));
		
		{
			HR(SwapChain1->QueryInterface(__uuidof(IDXGISwapChain), reinterpret_cast<void**>(mSwapChain.GetAddressOf())));
		}

		dxgiFactory2->Release();
	}

	ReleaseCOM(dxgiDevice);
	ReleaseCOM(dxgiAdaptor);
	ReleaseCOM(dxgiFactory);

	return true;
}

void SSDX11Renderer::Create2DLUTTexture()
{
	auto* deviceContext = GetImmediateDeviceContext();

	m2DLUTRenderTarget->Clear(deviceContext);
	m2DLUTRenderTarget->SetCurrentRenderTarget(deviceContext);

	SSDrawCommand blitDrawCmd{ m2DLUTVertexShader, m2DLUTPixelShader, mScreenBlit };
	blitDrawCmd.Do(deviceContext);

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

		convolutionDrawCmd.Do(deviceContext);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		convolutionDrawCmd.Do(deviceContext);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		convolutionDrawCmd.Do(deviceContext);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		convolutionDrawCmd.Do(deviceContext);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		convolutionDrawCmd.Do(deviceContext);

		mConvolutionRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		convolutionDrawCmd.Do(deviceContext);

		SSRasterizeStateManager::Get().SetToDefault(deviceContext);

		mConvolutionRenderTarget->CreateCubemapShaderResource(deviceContext);

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

			prefilterDrawCmd.Do(deviceContext);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
			prefilterDrawCmd.Do(deviceContext);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
			prefilterDrawCmd.Do(deviceContext);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
			prefilterDrawCmd.Do(deviceContext);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
			prefilterDrawCmd.Do(deviceContext);

			mPrefilterRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
			prefilterDrawCmd.Do(deviceContext);
		}
		SSRasterizeStateManager::Get().SetToDefault(GetImmediateDeviceContext());

		mPrefilterRenderTarget->CreateCubemapShaderResource(deviceContext);

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
	m2DLUTTexture = SSTexture2D::CreateFromDDSFile(GetImmediateDeviceContext(), L"./Prebaked/2DLUT.dds");
	return m2DLUTTexture != nullptr;
}

void SSDX11Renderer::CreateEnvCubemap()
{
	auto* deviceContext = GetImmediateDeviceContext();

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);

	mHDREnvmap = SSTexture2D::CreateFromHDRFile(deviceContext, "./Resource/Tex/HDR/Circus_Backstage_3k.hdr");
	{
        SSDrawCommand equirectToCubeDrawCmd{ mEquirectToCubemapVertexShader, mEquirectToCubemapPixelShader, mRenderTargetCube };

		mEquirectToCubemapRenderTarget->Clear(deviceContext);
		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_X);

		equirectToCubeDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		equirectToCubeDrawCmd.SetPSTexture("sampleTexture", mHDREnvmap.get());

		SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

		equirectToCubeDrawCmd.Do(deviceContext);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		equirectToCubeDrawCmd.Do(deviceContext);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		equirectToCubeDrawCmd.Do(deviceContext);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_X);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		equirectToCubeDrawCmd.Do(deviceContext);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::NEGATIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		equirectToCubeDrawCmd.Do(deviceContext);

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(deviceContext, ECubemapFace::POSITIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		equirectToCubeDrawCmd.Do(deviceContext);

		SSRasterizeStateManager::Get().SetToDefault(deviceContext);

		mEquirectToCubemapRenderTarget->CreateCubemapShaderResource(deviceContext);

		mEquirectToCubemapRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvCubemap.dds");

		mEnvCubemap = mEquirectToCubemapRenderTarget;
	}
}



void SSDX11Renderer::DrawScene()
{	
	DrawCubeScene();
	//DrawSponzaScene();
	//DrawDummyScene();
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

    skyboxCmd.Do(deviceContext);

    SSDepthStencilStateManager::Get().SetToDefault(deviceContext);
    SSRasterizeStateManager::Get().SetToDefault(deviceContext);
}


