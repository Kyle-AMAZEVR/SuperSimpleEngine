
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

SSDX11Renderer::SSDX11Renderer()	
{

}


void SSDX11Renderer::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;
	CreateDevice();
	CreateSwapChain();

	bInitialized = true;

	SSShaderManager::Get().Initialize();

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

	OnWindowResize(mBufferWidth, mBufferHeight);

	SSSamplerManager::Get().Initialize();
	SSDepthStencilStateManager::Get().Initialize();
	SSRasterizeStateManager::Get().Initialize();


	TestCompileShader();
	TestCreateResources();

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
	mTestVertexBuffer = std::make_shared<SSDX11VertexBuffer>();
	mTestIndexBuffer = std::make_shared<SSIndexBuffer>();

	auto* deviceContext = GetImmediateDeviceContext();

	mNormalTexture = std::make_shared<SSTexture2D>();
	mRoughnessTexture = std::make_shared<SSTexture2D>();
	mDiffuseTexture = std::make_shared<SSTexture2D>();
	mMetalicTexture = std::make_shared<SSTexture2D>();



	mTestCube = std::make_shared<SSCube>();
	mTestCubeTexture = std::make_shared<SSTextureCube>();
	mTestSphere = std::make_shared<SSSphere>(25, 25, 10.0f);
	mTestSphere2 = std::make_shared<SSSphere>(25, 25, 10.0f);
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

	mSponzaMesh = std::make_shared<SSObjMesh>();
	mObjMeshSphere = std::make_shared<SSObjMesh>();

	mTestCube->SetScale(1, 1, 1);
	mScreenBlit = std::make_shared<class SSScreenBlit>();

	if (SSFileHelper::FileExists(L"./Prebaked/sponza.mesh"))
		//if(false)
	{
		mSponzaMesh->LoadCookedFile("./Prebaked/sponza.mesh");
	}
	else
	{

		mSponzaMesh->ImportObjFile("./Resource/ObjMesh/sponza2.obj", "./Resource/ObjMesh/sponza2.mtl");
	}



	mNormalTexture->LoadFromDDSFile(GetImmediateDeviceContext(), L"./Resource/Tex/rustediron/rustediron2_normal.dds");
	mRoughnessTexture->LoadFromDDSFile(GetImmediateDeviceContext(), L"./Resource/Tex/rustediron/rustediron2_roughness.dds");
	mDiffuseTexture->LoadFromDDSFile(GetImmediateDeviceContext(), L"./Resource/Tex/rustediron/rustediron2_basecolor.dds");
	mMetalicTexture->LoadFromDDSFile(GetImmediateDeviceContext(), L"./Resource/Tex/rustediron/rustediron2_metallic.dds");

	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");
}

void SSDX11Renderer::TestCompileShader()
{
	mTestVertexShader = SSShaderManager::Get().GetVertexShader("Screen.vs");
	mTestPixelShader = SSShaderManager::Get().GetPixelShader("Screen.ps");

	mDeferredVertexShader = SSShaderManager::Get().GetVertexShader("GBuffer.vs");
	mDeferredPixelShader = SSShaderManager::Get().GetPixelShader("GBuffer.ps");

	mTestMaterial = std::make_shared<SSMaterial>(mDeferredVertexShader.get(), mDeferredPixelShader.get());

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

	mTBNDebugMaterial = std::make_shared<SSMaterial>(mTBNDebugVertexShader.get(), mTBNDebugPixelShader.get());
	mDeferredLightMaterial = std::make_shared<SSMaterial>(mDeferredLightVertexShader.get(), mDeferredLightPixelShader.get());

	//mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

bool SSDX11Renderer::CreateDevice()
{
	//
	D3D_FEATURE_LEVEL featureLevel;
	HR(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mDeviceContext.ReleaseAndGetAddressOf()));

	HR(mDevice->CreateDeferredContext(0, mDeferredContext.ReleaseAndGetAddressOf()));

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

		mBufferWidth = newWidth;
		mBufferHeight = newHeight;

		mViewport->Resize(newWidth, newHeight);
		mGBuffer->Resize(newWidth, newHeight);

		mFXAAPostProcess->OnResize(newWidth, newHeight);
		mGBufferDumpProcess->OnResize(newWidth, newHeight);
		mDeferredLightPostProcess->OnResize(newWidth, newHeight);
	}
}


bool SSDX11Renderer::CreateSwapChain()
{
	HR(mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4, &m4xMSAAQuality));

	DXGI_SWAP_CHAIN_DESC sd;

	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMSAAQuality - 1;

	sd.BufferDesc.Width = mBufferWidth;
	sd.BufferDesc.Height = mBufferHeight;
	sd.BufferDesc.RefreshRate.Numerator = 240;
	sd.BufferDesc.RefreshRate.Denominator = 1;

	sd.BufferDesc.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;


	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = mWindowHandle;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	HR(mDevice->QueryInterface(__uuidof(ID3D11Debug), (void**)&mDebug));

	IDXGIDevice* dxgiDevice = nullptr;
	HR(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&dxgiDevice));

	IDXGIAdapter* dxgiAdaptor = nullptr;
	HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdaptor));

	IDXGIFactory* dxgiFactory = nullptr;
	HR(dxgiAdaptor->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory));

	HR(dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, &mSwapChain));

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

	SSDrawCommand blitDrawCmd{ m2DLUTVertexShader.get(), m2DLUTPixelShader.get(), mScreenBlit };
	blitDrawCmd.Do(deviceContext);

	m2DLUTRenderTarget->SaveRTTexture(0, L"./Prebaked/2DLUT.dds");
}

void SSDX11Renderer::CreateEnvCubemapConvolution()
{
	auto* deviceContext = GetImmediateDeviceContext();

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);
	{
		SSDrawCommand convolutionDrawCmd{ mCubemapConvolutionVertexShader.get(), mCubemapConvolutionPixelShader.get(), mTestCube };

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
		SSDrawCommand prefilterDrawCmd{ mPrefilterVertexShader.get(), mPrefilterPixelShader.get(), mTestCube };

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
		SSDrawCommand equirectToCubeDrawCmd{ mEquirectToCubemapVertexShader.get(), mEquirectToCubemapPixelShader.get(), mTestCube };

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
	if (bInitialized == false)
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


	// @draw cubemap to gbuffer
	// @start
	mGBuffer->Clear(deviceContext);
	mGBuffer->SetCurrentRenderTarget(deviceContext);
	SSCameraManager::Get().UpdateCurrentCamera();

	SSDrawCommand testDrawCmd{ mCubemapVertexShader.get(), mCubemapPixelShader.get(), mTestSphere };

	XMMATRIX mvp = SSCameraManager::Get().GetCurrentCameraMVP();

	testDrawCmd.StoreVSConstantBufferData(MVPName, XMMatrixTranspose(mvp));
	testDrawCmd.SetPSTexture("gCubeMap", mEnvCubemapPrefilter.get());

	SSDepthStencilStateManager::Get().SetDepthCompLessEqual(deviceContext);
	SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

	testDrawCmd.Do(deviceContext);

	SSDepthStencilStateManager::Get().SetToDefault(deviceContext);
	SSRasterizeStateManager::Get().SetToDefault(deviceContext);
	SSRasterizeStateManager::Get().SetCullModeNone(deviceContext);

	//mText3D->Draw(deviceContext, mTBNDebugMaterial.get());
	SSRasterizeStateManager::Get().SetToDefault(deviceContext);

	mTestSphere->Draw(deviceContext, mTestMaterial.get());

	mRustedIron->Draw(deviceContext, mTestMaterial.get());

	mTile->Draw(deviceContext, mTestMaterial.get());

	mMetalGrid->Draw(deviceContext, mTestMaterial.get());

	mSponzaMesh->Draw(deviceContext, mTestMaterial.get());

	mFXAAPostProcess->Draw(deviceContext, mDeferredLightPostProcess->GetOutput(0));

	mGBufferDumpProcess->Draw(deviceContext, mGBuffer->GetPositionOutput(), mGBuffer->GetColorOutput(), mGBuffer->GetNormalOutput());

	mDeferredLightPostProcess->Draw(
		deviceContext,
		mGBuffer->GetPositionOutput(),
		mGBuffer->GetColorOutput(),
		mGBuffer->GetNormalOutput(),
		m2DLUTTexture.get(),
		mEnvCubemapConvolution.get(),
		mEnvCubemapPrefilter.get());

	mViewport->Clear(deviceContext);
	mViewport->SetCurrentRenderTarget(deviceContext);

	SSDrawCommand blitDrawCmd{ mTestVertexShader.get(), mTestPixelShader.get(), mScreenBlit };

	if (bGbufferDump)
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mGBufferDumpProcess->GetOutput(0));
	}
	else
	{
		blitDrawCmd.SetPSTexture("sampleTexture", mFXAAPostProcess->GetOutput(0));
	}

	blitDrawCmd.Do(deviceContext);

	HR(mSwapChain->Present(0, 0));
}


