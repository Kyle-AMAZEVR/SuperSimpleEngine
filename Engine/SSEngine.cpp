
#include "Core.h"
#include "Util.h"
#include "SSEngine.h"
#include "SSShader.h"
#include "FreqUsedConstantBufferTypes.h"
#include "CameraManager.h"
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSTexture2D.h"
#include "SSCube.h"
#include "SSSamplerManager.h"
#include "SSDrawCommand.h"
#include "SSGBuffer.h"
#include "SSScreenBlit.h"
#include "SSRenderTarget2D.h"
#include "SSTextureCube.h"
#include "SSSphere.h"
#include "SSDepthStencilStateManager.h"
#include "SSRasterizeStateManager.h"
#include "SSCubemapRenderTarget.h"
#include "SSMathHelper.h"
#include "SSFreqUsedNames.h"
#include "SSFileHelper.h"
#include "SSObjMesh.h"
#include "SSShaderManager.h"

bool SSEngine::bInitialized = false;

bool SSEngine::Initialize(HWND windowHandle)
{
    mWindowHandle = windowHandle;
    CreateDevice();
    CreateSwapChain();	

	bInitialized = true;

	mViewport = std::make_shared<SSViewport>();
	mGBuffer = std::make_shared<SSGBuffer>(1024, 768);
	
	mEquirectToCubemapRenderTarget = std::make_shared<SSCubemapRenderTarget>(1024,1024);
	mConvolutionRenderTarget = std::make_shared<SSCubemapRenderTarget>(512, 512);
	mPrefilterRenderTarget = std::make_shared<SSPrefilterCubemapRenderTarget>(1024, 1024,5);
	m2DLUTRenderTarget = std::make_shared<class SSGenericRenderTarget>(512, 512, 1, false);
	mFXAARenderTarget = std::make_shared<SSGenericRenderTarget>(1024, 768, 1, false);
	

	if(SSFileHelper::DirectoryExists(L"./Prebaked") == false)
	{
		SSFileHelper::MakeDirectory(L"./Prebaked");
	}

    OnWindowResize(mBufferWidth, mBufferHeight);	
	
	
	SSSamplerManager::Get().Initialize();
	SSDepthStencilStateManager::Get().Initialize();
	SSRaterizeStateManager::Get().Initialize();
	SSShaderManager::Get().Initialize();

    TestCompileShader();
    TestCreateResources();

    return true;
}

void SSEngine::Shutdown()
{
	SSDepthStencilStateManager::Get().Shutdown();
	SSRaterizeStateManager::Get().Shutdown();
	SSSamplerManager::Get().Shutdown();
	SSShaderManager::Get().Shutdown();

	ReleaseCOM(mSwapChain);
	ReleaseCOM(mDevice);
	ReleaseCOM(mDeviceContext);
}

void SSEngine::TestCreateResources()
{   
    mTestVertexBuffer = std::make_shared<SSVertexBuffer>();
    mTestIndexBuffer = std::make_shared<SSIndexBuffer>();
	mTestTexture = std::make_shared<SSTexture2D>();
	mTestCube = std::make_shared<SSCube>();
	mTestCubeTexture = std::make_shared<SSTextureCube>();
	mTestSphere = std::make_shared<SSSphere>(20, 20, 2.0f);	
	mObjMesh = std::make_shared<SSObjMesh>();
	mTestCube->SetScale(1, 1, 1);
	mScreenBlit = std::make_shared<class SSScreenBlit>();
	
	//mObjMesh->ImportObjFile("./Resource/ObjMesh/pistol.obj", "./Resource/ObjMesh/pistol.mtl");
	mObjMesh->LoadCookedFile("./Prebaked/pistol.mesh");
	mObjMesh->SetScale(0.1f, 0.1f,0.1f);

	mTestTexture->LoadFromDDSFile(L"./Resource/Tex/pistol/Cerberus_A.dds");
	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");	
}

void SSEngine::TestCompileShader()
{   
    mTestVertexShader = SSShaderManager::Get().GetVertexShader("Screen.vs");
	mTestPixelShader = SSShaderManager::Get().GetPixelShader("Screen.ps");
	
	mDeferredVertexShader = SSShaderManager::Get().GetVertexShader("DeferredShader.vs");
	mDeferredPixelShader = SSShaderManager::Get().GetPixelShader("DeferredShader.ps");

	mCubemapVertexShader = SSShaderManager::Get().GetVertexShader("CubemapShader.vs");
	mCubemapPixelShader = SSShaderManager::Get().GetPixelShader("CubemapShader.ps");
	
	mEquirectToCubemapVertexShader = SSShaderManager::Get().GetVertexShader("EquirectangleToCubemap.vs");
	mEquirectToCubemapPixelShader = SSShaderManager::Get().GetPixelShader("EquirectangleToCubemap.ps");
	
	mCubemapConvolutionVertexShader = SSShaderManager::Get().GetVertexShader("CubemapConvolution.vs");
	mCubemapConvolutionPixelShader = SSShaderManager::Get().GetPixelShader("CubemapConvolution.ps");

	mPrefilterVertexShader = SSShaderManager::Get().GetVertexShader("Prefilter.vs");
	mPrefilterPixelShader = SSShaderManager::Get().GetPixelShader("Prefilter.ps");

	m2DLUTVertexShader = SSShaderManager::Get().GetVertexShader("2DLUT.vs");
	m2DLUTPixelShader = SSShaderManager::Get().GetPixelShader("2DLUT.ps");

	mFXAAVertexShader = SSShaderManager::Get().GetVertexShader("FXAA.vs");
	mFXAAPixelShader = SSShaderManager::Get().GetPixelShader("FXAA.ps");
    
    //mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
}

bool SSEngine::CreateDevice()
{
    //
    D3D_FEATURE_LEVEL featureLevel;        
    HR(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext));     
    return true;
}

void SSEngine::OnWindowResize(int newWidth, int newHeight)
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
		mFXAARenderTarget->Resize(newWidth, newHeight);
	}
}


bool SSEngine::CreateSwapChain()
{
    HR(mDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT,4,&m4xMSAAQuality));    

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

    HR(mDevice->QueryInterface(__uuidof(ID3D11Debug) , (void**) &mDebug));
    
    IDXGIDevice* dxgiDevice = nullptr;
    HR(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**) &dxgiDevice));    

    IDXGIAdapter* dxgiAdaptor = nullptr;
    HR(dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**) &dxgiAdaptor));

    IDXGIFactory* dxgiFactory = nullptr;
    HR(dxgiAdaptor->GetParent(__uuidof(IDXGIFactory), (void**) &dxgiFactory));

    HR(dxgiFactory->CreateSwapChain(mDevice, &sd, &mSwapChain));

    ReleaseCOM(dxgiDevice);
    ReleaseCOM(dxgiAdaptor);
    ReleaseCOM(dxgiFactory);

    return true;
}

void SSEngine::Create2DLUTTexture()
{
	m2DLUTRenderTarget->Clear();
	m2DLUTRenderTarget->SetCurrentRenderTarget();

	SSDrawCommand blitDrawCmd{ m2DLUTVertexShader.get(), m2DLUTPixelShader.get(), mScreenBlit };	
	blitDrawCmd.Do();

	m2DLUTRenderTarget->SaveRTTexture(0, L"./Prebaked/2DLUT.dds");
}

void SSEngine::CreateEnvCubemapConvolution()
{
	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);
	{
		SSDrawCommand convolutionDrawCmd{ mCubemapConvolutionVertexShader.get(), mCubemapConvolutionPixelShader.get(), mTestCube };

		mConvolutionRenderTarget->Clear();
		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_X);

		convolutionDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
		convolutionDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		convolutionDrawCmd.SetPSTexture("EnvironmentMap", mEnvCubemap.get());

		SSRaterizeStateManager::Get().SetCullModeNone();

		convolutionDrawCmd.Do();

		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		convolutionDrawCmd.Do();

		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Y);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		convolutionDrawCmd.Do();

		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_X);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		convolutionDrawCmd.Do();

		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		convolutionDrawCmd.Do();

		mConvolutionRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Z);
		convolutionDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		convolutionDrawCmd.Do();

		SSRaterizeStateManager::Get().SetToDefault();

		mConvolutionRenderTarget->CreateCubemapShaderResource();	

		mEnvCubemapConvolution = mConvolutionRenderTarget;

		mConvolutionRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvConvolution.dds");
	}
}

void SSEngine::CreateEnvCubemapPrefilter()
{
	XMFLOAT3 origin = XMFLOAT3(0, 0, 0); 
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);
	{
		SSDrawCommand prefilterDrawCmd{ mPrefilterVertexShader.get(), mPrefilterPixelShader.get(), mTestCube };

		mPrefilterRenderTarget->Clear();

		SSRaterizeStateManager::Get().SetCullModeNone();

		const int maxMipLevels = 5;


		for (UINT mip = 0; mip < 5; ++mip)
		{
			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_X, mip);

			prefilterDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
			prefilterDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
			CbufferFloat temp;
			temp.value = (float)mip / (float)(maxMipLevels - 1);


			prefilterDrawCmd.StorePSConstantBufferData(RoughnessName, temp);
			prefilterDrawCmd.SetPSTexture("EnvironmentMap", mEquirectToCubemapRenderTarget.get());

			prefilterDrawCmd.Do();

			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
			prefilterDrawCmd.Do();

			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
			prefilterDrawCmd.Do();

			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_X, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
			prefilterDrawCmd.Do();

			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Y, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
			prefilterDrawCmd.Do();

			mPrefilterRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Z, mip);
			prefilterDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
			prefilterDrawCmd.Do();
		}
		SSRaterizeStateManager::Get().SetToDefault();

		mPrefilterRenderTarget->CreateCubemapShaderResource();

		mEnvCubemapPrefilter = mPrefilterRenderTarget;
		
		mPrefilterRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvPrefilter.dds");
	}
}

bool SSEngine::TryLoadEnvCubemap(std::wstring filepath) 
{
	mEnvCubemap = SSTextureCube::CreateFromDDSFile(filepath);	
	return mEnvCubemap != nullptr;
}

bool SSEngine::TryLoadEnvCubemapPrefilter(std::wstring filepath)
{
	mEnvCubemapPrefilter = SSTextureCube::CreateFromDDSFile(filepath);
	return mEnvCubemapPrefilter != nullptr;
}

bool SSEngine::TryLoadEnvCubemapConvolution(std::wstring filepath)
{
	mEnvCubemapConvolution = SSTextureCube::CreateFromDDSFile(filepath);
	return mEnvCubemapConvolution != nullptr;
}


bool SSEngine::TryLoad2DLUTTexture()
{
	m2DLUTTexture = SSTexture2D::CreateFromDDSFile(L"./Prebaked/2DLUT.dds");
	return m2DLUTTexture != nullptr;
}

void SSEngine::CreateEnvCubemap()
{
	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);
	auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);	
	
	{
		SSDrawCommand equirectToCubeDrawCmd{ mEquirectToCubemapVertexShader.get(), mEquirectToCubemapPixelShader.get(), mTestCube };

		mEquirectToCubemapRenderTarget->Clear();
		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_X);

		equirectToCubeDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveXViewMatrix));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		equirectToCubeDrawCmd.SetPSTexture("sampleTexture", mTestTexture.get());

		SSRaterizeStateManager::Get().SetCullModeNone();

		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveYViewMatrix));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Y);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeYViewMatrix));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_X);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeXViewMatrix));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::NEGATIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::NegativeZViewMatrix));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAs(ECubemapFace::POSITIVE_Z);
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSMathHelper::PositiveZViewMatrix));
		equirectToCubeDrawCmd.Do();

		SSRaterizeStateManager::Get().SetToDefault();

		mEquirectToCubemapRenderTarget->CreateCubemapShaderResource();
		
		mEquirectToCubemapRenderTarget->SaveAsCubemapDDSFile(L"./Prebaked/EnvCubemap.dds");

		mEnvCubemap = mEquirectToCubemapRenderTarget;
	}
}

void SSEngine::DrawScene()
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
		if (TryLoad2DLUTTexture()==false)
		{
			Create2DLUTTexture();			
		}		
		bLUTCreated = true;
	}

	// @end


	// @draw cubemap to gbuffer
	// @start
	mGBuffer->Clear();
	mGBuffer->SetCurrentRenderTarget();
	
	SSDrawCommand testDrawCmd{ mCubemapVertexShader.get(), mCubemapPixelShader.get(), mTestSphere };
	SSCameraManager::Get().UpdateCurrentCamera();
	
	XMMATRIX scale = XMMatrixScaling(2, 2, 2) * SSCameraManager::Get().GetCurrentCameraTranslation();
	XMMATRIX modelView = scale * SSCameraManager::Get().GetCurrentCameraView();
	XMMATRIX mvp = modelView * SSCameraManager::Get().GetCurrentCameraProj();

	testDrawCmd.StoreVSConstantBufferData(MVPName, XMMatrixTranspose(mvp));	
	testDrawCmd.SetPSTexture("gCubeMap", mEnvCubemapPrefilter.get());

	testDrawCmd.SetPreDrawJob([]()
	{
		SSDepthStencilStateManager::Get().SetDepthCompLessEqual();
		SSRaterizeStateManager::Get().SetCullModeNone();
	});

	testDrawCmd.SetPostDrawJob([]()
	{
		SSDepthStencilStateManager::Get().SetToDefault();
		SSRaterizeStateManager::Get().SetToDefault();
	});
	
	testDrawCmd.Do();

	// @end

	SSDrawCommand sphereDrawCmd{ mDeferredVertexShader.get(), mDeferredPixelShader.get(), mObjMesh };

	sphereDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixScaling(0.1,0.1,0.1) * XMMatrixTranslation(10, 0, 0)));
	sphereDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	sphereDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));
	sphereDrawCmd.SetPSTexture("sampleTexture", mTestTexture.get());
	
	sphereDrawCmd.Do();	

	mFXAARenderTarget->Clear();
	mFXAARenderTarget->SetCurrentRenderTarget();

	SSDrawCommand fxaaDrawCmd{ mFXAAVertexShader.get(), mFXAAPixelShader.get(), mScreenBlit };
	fxaaDrawCmd.SetPSTexture("ScreenTex", mGBuffer->GetColorOutput());
	CbufferInvScreenSize invScreenSize;
	invScreenSize.InvScreenSize.x = 1 / static_cast<float>(mBufferWidth);
	invScreenSize.InvScreenSize.y = 1 / static_cast<float>(mBufferHeight);
	fxaaDrawCmd.StorePSConstantBufferData("CBInverseScreenSize", invScreenSize);
	fxaaDrawCmd.Do();

	mViewport->Clear();
	mViewport->SetCurrentRenderTarget();

	SSDrawCommand blitDrawCmd{ mTestVertexShader.get(), mTestPixelShader.get(), mScreenBlit };
	blitDrawCmd.SetPSTexture("sampleTexture", mFXAARenderTarget->GetOutput(0));
	blitDrawCmd.Do();
	
    HR(mSwapChain->Present(0,0));
}


