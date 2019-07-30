
#include "Core.h"
#include "Util.h"
#include "SSEngine.h"
#include "SSShader.h"
#include "DXVertexTypes.h"
#include "SSVertexElementDeclaration.h"
#include "FreqUsedConstantBufferTypes.h"
#include "CameraManager.h"
#include "DXMathHelper.h"
#include "DXFreeCamera.h"
#include "DXVertexBuffer.h"
#include "SSIndexBuffer.h"
#include "SSTexture2D.h"
#include "SSCube.h"
#include "SSSamplerManager.h"
#include "SSDrawCommand.h"
#include "SSGBuffer.h"
#include "SSScreenBlit.h"
#include "SSRenderTarget2D.h"

bool SSEngine::bInitialized = false;

bool SSEngine::Initialize(HWND windowHandle)
{
    mWindowHandle = windowHandle;
    CreateDevice();
    CreateSwapChain();	

	bInitialized = true;

	mViewport = std::make_shared<SSViewport>();

    OnWindowResize(mBufferWidth, mBufferHeight);	
	
	mGBuffer = std::make_shared<SSGBuffer>(1024, 768);
	SSSamplerManager::Get().Initialize();
    TestCompileShader();
    TestCreateResources();

    return true;
}

void SSEngine::Shutdown()
{
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

	mTestCube->SetScale(1, 1, 1);
	mScreenBlit = std::make_shared<class SSScreenBlit>();
    
	mTestTexture->LoadFromDDSFile(L"./Resource/Tex/rustediron2_basecolor.dds");
}

void SSEngine::TestCompileShader()
{    
    mTestVertexShader = std::make_shared<SSVertexShader>();
	mTestPixelShader = std::make_shared<SSPixelShader>();
	mDeferredVertexShader = std::make_shared<SSVertexShader>();
	mDeferredPixelShader = std::make_shared<SSPixelShader>();	

    assert(mDeferredVertexShader->CompileFromFile(L"./Shader/DeferredShader.vs"));
	assert(mDeferredPixelShader->CompileFromFile(L"./Shader/DeferredShader.ps"));
	
	assert(mTestVertexShader->CompileFromFile(L"./Shader/Screen.vs"));    
	assert(mTestPixelShader->CompileFromFile(L"./Shader/Screen.ps"));
    
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

void SSEngine::SetVertexShader(std::weak_ptr<SSVertexShader> vs)
{
	if (vs.expired() == false)
	{
		auto sharedVS = vs.lock();
		mDeviceContext->VSSetShader(sharedVS->GetShader(), nullptr, 0);
	}
}
void SSEngine::SetPixelShader(std::weak_ptr<SSPixelShader> ps)
{
	
}

void SSEngine::DrawScene()
{
    if(bInitialized == false)
    {
        return;
    }
    
    check(mDeviceContext != nullptr);

	mGBuffer->Clear();
	mGBuffer->MakeCurrentRenderTarget();
	//
	

	//SSDrawCommand testDrawCmd{ mTestVertexShader.get(), mTestPixelShader.get(), mTestCube };
	SSDrawCommand testDrawCmd{ mDeferredVertexShader.get(), mDeferredPixelShader.get(), mTestCube };

	SSCameraManager::Get().UpdateCurrentCamera();

	XMFLOAT4X4 model; XMStoreFloat4x4(&model, XMMatrixTranspose(mTestCube->GetModelTransform()));
	XMFLOAT4X4 view; XMStoreFloat4x4(&view, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraView()));
	XMFLOAT4X4 proj; XMStoreFloat4x4(&proj, XMMatrixTranspose(SSCameraManager::Get().GetCurrentCameraProj()));
		
	testDrawCmd.StoreVSConstantBufferData("Model", model);
	testDrawCmd.StoreVSConstantBufferData("View", view);
	testDrawCmd.StoreVSConstantBufferData("Proj", proj);
	
	testDrawCmd.SetPSTexture("sampleTexture", mTestTexture.get());

	testDrawCmd.Do();

	mViewport->Clear();
	mViewport->MakeCurrentRenderTarget();

	SSDrawCommand blitDrawCmd{ mTestVertexShader.get(), mTestPixelShader.get(), mScreenBlit };

	blitDrawCmd.SetPSTexture("sampleTexture", mGBuffer->GetColorOutput());
	
	blitDrawCmd.Do();

	
    HR(mSwapChain->Present(0,0));
}