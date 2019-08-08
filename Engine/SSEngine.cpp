
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

    OnWindowResize(mBufferWidth, mBufferHeight);	
	
	
	SSSamplerManager::Get().Initialize();
	SSDepthStencilStateManager::Get().Initialize();
	SSRaterizeStateManager::Get().Initialize();

    TestCompileShader();
    TestCreateResources();

    return true;
}

void SSEngine::Shutdown()
{
	SSDepthStencilStateManager::Get().Shutdown();
	SSRaterizeStateManager::Get().Shutdown();
	SSSamplerManager::Get().Shutdown();

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

	mTestCube->SetScale(1, 1, 1);
	mScreenBlit = std::make_shared<class SSScreenBlit>();
	
	mTestTexture->LoadFromHDRFile(L"./Resource/Tex/HDR/Ueno-Shrine_3k.hdr");
	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");	
}

void SSEngine::TestCompileShader()
{    
    mTestVertexShader = std::make_shared<SSVertexShader>();
	mTestPixelShader = std::make_shared<SSPixelShader>();
	mDeferredVertexShader = std::make_shared<SSVertexShader>();
	mDeferredPixelShader = std::make_shared<SSPixelShader>();	
	mCubemapVertexShader = std::make_shared<SSVertexShader>();
	mCubemapPixelShader = std::make_shared<SSPixelShader>();	
	
	mEquirectToCubemapVertexShader = std::make_shared<SSVertexShader>();
	mEquirectToCubemapPixelShader = std::make_shared<SSPixelShader>();
	
	mCubemapConvolutionVertexShader = std::make_shared<SSVertexShader>();
	mCubemapConvolutionPixelShader = std::make_shared<SSPixelShader>();

	mPrefilterVertexShader = std::make_shared<SSVertexShader>();
	mPrefilterPixelShader = std::make_shared<SSPixelShader>();

	m2DLUTPixelShader = std::make_shared<SSPixelShader>();
	m2DLUTVertexShader = std::make_shared<SSVertexShader>();

    assert(mDeferredVertexShader->CompileFromFile(L"./Shader/DeferredShader.vs"));
	assert(mDeferredPixelShader->CompileFromFile(L"./Shader/DeferredShader.ps"));
	
	assert(mTestVertexShader->CompileFromFile(L"./Shader/Screen.vs"));    
	assert(mTestPixelShader->CompileFromFile(L"./Shader/Screen.ps"));

	assert(mCubemapVertexShader->CompileFromFile(L"./Shader/CubemapShader.vs"));
	assert(mCubemapPixelShader->CompileFromFile(L"./Shader/CubemapShader.ps"));

	assert(mEquirectToCubemapVertexShader->CompileFromFile(L"./Shader/EquirectangleToCubemap.vs"));
	assert(mEquirectToCubemapPixelShader->CompileFromFile(L"./Shader/EquirectangleToCubemap.ps"));

	assert(mCubemapConvolutionVertexShader->CompileFromFile(L"./Shader/CubemapConvolution.vs"));
	assert(mCubemapConvolutionPixelShader->CompileFromFile(L"./Shader/CubemapConvolution.ps"));

	assert(mPrefilterVertexShader->CompileFromFile(L"./Shader/Prefilter.vs"));
	assert(mPrefilterPixelShader->CompileFromFile(L"./Shader/Prefilter.ps"));

	assert(m2DLUTVertexShader->CompileFromFile(L"./Shader/2DLUT.vs"));
	assert(m2DLUTPixelShader->CompileFromFile(L"./Shader/2DLUT.ps"));
    
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
		assert(mDeviceContext);
		assert(mDevice);
		assert(mSwapChain);

		check(newWidth > 0 && newHeight > 0);

		mBufferWidth = newWidth;
		mBufferHeight = newHeight;

		mViewport->Resize(newWidth, newHeight);
		mGBuffer->Resize(newWidth, newHeight);
		
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

	m2DLUTRenderTarget->SaveRTTexture(0, L"LUT.dds");
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
		convolutionDrawCmd.SetPSTexture("EnvironmentMap", mEquirectToCubemapRenderTarget.get());

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

		mPrefilterRenderTarget->SaveFaceOfMipAsDDSFile(ECubemapFace::NEGATIVE_X, 1);
		mPrefilterRenderTarget->SaveFaceAsDDSFile(ECubemapFace::NEGATIVE_X);
		mPrefilterRenderTarget->SaveAsCubemapDDSFile();		
	}
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

		mEquirectToCubemapRenderTarget->SaveFaceOfMipAsDDSFile(ECubemapFace::NEGATIVE_X, 1);
		mEquirectToCubemapRenderTarget->SaveFaceAsDDSFile(ECubemapFace::NEGATIVE_X);
		mEquirectToCubemapRenderTarget->SaveAsCubemapDDSFile();
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
		CreateEnvCubemap();
		bEquidirectToCubeDrawn = true;		
	}

	if (bConvolutionDrawn == false)
	{
		CreateEnvCubemapConvolution();
		bConvolutionDrawn = true;
	}

	if (bPrefilterDrawn == false)
	{
		CreateEnvCubemapPrefilter();
		bPrefilterDrawn = true;
	}

	if (bLUTCreated == false)
	{
		Create2DLUTTexture();
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
	testDrawCmd.SetPSTexture("gCubeMap", mPrefilterRenderTarget.get());

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

	SSDrawCommand sphereDrawCmd{ mDeferredVertexShader.get(), mDeferredPixelShader.get(), mTestSphere };

	sphereDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(10, 0, 0)));
	sphereDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	sphereDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));
	sphereDrawCmd.SetPSTexture("sampleTexture", mTestTexture.get());
	
	sphereDrawCmd.Do();	

	mViewport->Clear();
	mViewport->SetCurrentRenderTarget();

	SSDrawCommand blitDrawCmd{ mTestVertexShader.get(), mTestPixelShader.get(), mScreenBlit };
	blitDrawCmd.SetPSTexture("sampleTexture", mGBuffer->GetColorOutput());
	blitDrawCmd.Do();
	
    HR(mSwapChain->Present(0,0));
}


