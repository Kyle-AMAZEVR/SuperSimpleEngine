
#include "Core.h"
#include "SSEngine.h"
#include "SSTexture2D.h"
#include "SSObjMesh.h"
#include "SSDX11Renderer.h"

bool SSDX11Engine::bInitialized = false;

SSDX11Engine* SSDX11Engine::mInstance = nullptr;

SSDX11Engine& SSDX11Engine::Get()
{
	if(mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return (*mInstance);
}

SSDX11Engine* SSDX11Engine::GetPtr()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return mInstance;
}

ID3D11Device* SSDX11Engine::GetDevice() const
{
	return mRenderer->GetDevice();
}

ID3D11DeviceContext* SSDX11Engine::GetImmediateDeviceContext() const
{
	return mRenderer->GetImmediateDeviceContext();
}

inline IDXGISwapChain* SSDX11Engine::GetSwapChain() const
{
	return mRenderer->GetSwapChain();
}

void SSDX11Engine::Initialize(HWND windowHandle)
{
	mRenderer = new SSDX11Renderer();
	mRenderer->Initialize(windowHandle);

	mRenderingThread->SetRenderer(mRenderer);
	
	mGameThread = new SSGameThread(GetCurrentThreadId());
	
    mWindowHandle = windowHandle;

	bInitialized = true;
}

void SSDX11Engine::ToggleGBufferDumpMode()
{
	bGbufferDump = !bGbufferDump;
}

void SSDX11Engine::Shutdown()
{
	if (mRenderingThread)
	{
		// wait for rendering thread		
		mRenderingThread->RequestExit();
		mRenderingThread->Join();

		delete mRenderingThread;
		mRenderingThread = nullptr;
	}
	
	if(mRenderer)
	{
		mRenderer = nullptr;
	}

	if(mGameThread)
	{
		delete mGameThread;
		mGameThread = nullptr;
	}

	
}

bool SSDX11Engine::CreateDevice()
{
    //
    D3D_FEATURE_LEVEL featureLevel;        
    HR(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, mDevice.GetAddressOf(), &featureLevel, mDeviceContext.ReleaseAndGetAddressOf()));

	HR(mDevice->CreateDeferredContext(0, mDeferredContext.ReleaseAndGetAddressOf()));

    return true;
}

void SSDX11Engine::OnWindowResize(int newWidth, int newHeight)
{
	if (bInitialized)
	{
		if(mRenderer)
		{
			mRenderer->OnWindowResize(newWidth, newHeight);
		}
	}
}


bool SSDX11Engine::CreateSwapChain()
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

    HR(dxgiFactory->CreateSwapChain(mDevice.Get(), &sd, &mSwapChain));

    ReleaseCOM(dxgiDevice);
    ReleaseCOM(dxgiAdaptor);
    ReleaseCOM(dxgiFactory);

    return true;
}

void SSDX11Engine::Run()
{
	EngineStart();

	while(!bRequestExit)
	{
		mGameThread->Tick(0.033f);
	}

	Shutdown();
}

void SSDX11Engine::TickGameThread(float deltaTime)
{
	
}
void SSDX11Engine::TickRenderThread(float deltaTime)
{
	if (mRenderer)
	{
		mGameThread->WaitForGameThread(10);
		mRenderer->DrawScene();
	}
}

