
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
#include "DXMathHelper.h"
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
	mCubemapRenderTarget = std::make_shared<SSGenericRenderTarget>(1024, 768, 1, false);
	mEquirectToCubemapRenderTarget = std::make_shared<SSCubemapRenderTarget>(1024,1024);

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


    
	//mTestTexture->LoadFromDDSFile(L"./Resource/Tex/rustediron2_basecolor.dds");
	mTestTexture->LoadFromHDRFile(L"./Resource/Tex/HDR/Ueno-Shrine_3k.hdr");
	mTestCubeTexture->LoadFromDDSFile(L"./Resource/Tex/grasscube1024.dds");
	//mTestCubeTexture->LoadFromHDRFile(L"./Resource/Tex/HDR/Ueno-Shrine_3k.hdr");
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

    assert(mDeferredVertexShader->CompileFromFile(L"./Shader/DeferredShader.vs"));
	assert(mDeferredPixelShader->CompileFromFile(L"./Shader/DeferredShader.ps"));
	
	assert(mTestVertexShader->CompileFromFile(L"./Shader/Screen.vs"));    
	assert(mTestPixelShader->CompileFromFile(L"./Shader/Screen.ps"));

	assert(mCubemapVertexShader->CompileFromFile(L"./Shader/CubemapShader.vs"));
	assert(mCubemapPixelShader->CompileFromFile(L"./Shader/CubemapShader.ps"));

	assert(mEquirectToCubemapVertexShader->CompileFromFile(L"./Shader/EquirectangleToCubemap.vs"));
	assert(mEquirectToCubemapPixelShader->CompileFromFile(L"./Shader/EquirectangleToCubemap.ps"));
    
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
		mCubemapRenderTarget->Resize(newWidth, newHeight);
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

void SSEngine::DrawScene()
{
    if(bInitialized == false)
    {
        return;
    }
    
    check(mDeviceContext != nullptr);
	   
	// @equirect to cube
	// @start
	SSDrawCommand equirectToCubeDrawCmd{ mEquirectToCubemapVertexShader.get(), mEquirectToCubemapPixelShader.get(), mTestCube };

	XMFLOAT3 origin = XMFLOAT3(0, 0, 0);

	static bool bEquidirectToCubeDrawn = false;

	if (bEquidirectToCubeDrawn == false)
	{
		auto negXView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::MinusUnitX4), XMLoadFloat4(&DXMathHelper::MinusUnitY4));

		auto negYView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::UnitY4), XMLoadFloat4(&DXMathHelper::MinusUnitZ4));

		auto posXView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::UnitX4), XMLoadFloat4(&DXMathHelper::MinusUnitY4));		
				
		auto posYView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::MinusUnitY4), XMLoadFloat4(&DXMathHelper::MinusUnitZ4));

		auto negZView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::UnitZ4), XMLoadFloat4(&DXMathHelper::MinusUnitY4));

		auto posZView = XMMatrixLookToLH(XMLoadFloat3(&origin), XMLoadFloat4(&DXMathHelper::MinusUnitZ4), XMLoadFloat4(&DXMathHelper::MinusUnitY4));


		auto proj = XMMatrixPerspectiveFovLH(XMConvertToRadians(90.0f), 1.0f, 0.1f, 10.0f);

		mEquirectToCubemapRenderTarget->Clear();
		mEquirectToCubemapRenderTarget->SetCurrentRTAsPositiveX();

		equirectToCubeDrawCmd.StoreVSConstantBufferData(ModelName, XMMatrixTranspose(XMMatrixTranslation(0, 0, 0)));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(posXView));
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ProjName, XMMatrixTranspose(proj));
		equirectToCubeDrawCmd.SetPSTexture("sampleTexture", mTestTexture.get());		
		
		SSRaterizeStateManager::Get().SetCullModeNone();

		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAsPositiveY();
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(posYView));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAsNegativeY();
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(negYView));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAsNegativeX();
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(negXView));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAsNegativeZ();
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(negZView));
		equirectToCubeDrawCmd.Do();

		mEquirectToCubemapRenderTarget->SetCurrentRTAsPositiveZ();
		equirectToCubeDrawCmd.StoreVSConstantBufferData(ViewName, XMMatrixTranspose(posZView));
		equirectToCubeDrawCmd.Do();

		SSRaterizeStateManager::Get().SetToDefault();

		mEquirectToCubemapRenderTarget->CreateCubemapResource();
		bEquidirectToCubeDrawn = true;
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
	testDrawCmd.SetPSTexture("gCubeMap", mEquirectToCubemapRenderTarget.get());

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
