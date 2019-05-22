
#include "Core.h"
#include "Util.h"
#include "DXEngine.h"
#include "DXShader.h"


bool DXEngine::Initialize(HWND windowHandle)
{
    mWindowHandle = windowHandle;
    CreateDevice();
    CreateSwapChain();
    OnWindowResize(1024,768);
    bInitialized = true;

    TestCompileShader();
    return true;
}

void DXEngine::TestCompileShader()
{
    DXVertexShader shader;
    shader.CompileFromFile(L"./Shader/BasicShader.vs");
}

bool DXEngine::CreateDevice()
{
    //
    D3D_FEATURE_LEVEL featureLevel;        
    HR(D3D11CreateDevice(0, D3D_DRIVER_TYPE_HARDWARE, 0, D3D11_CREATE_DEVICE_DEBUG, 0, 0, D3D11_SDK_VERSION, &mDevice, &featureLevel, &mDeviceContext)); 
    
    return true;
}

void DXEngine::OnWindowResize(int newWidth, int newHeight)
{
    assert(mDeviceContext);
	assert(mDevice);
	assert(mSwapChain);

    check ( newWidth > 0 && newHeight > 0 );    
    mViewport.Resize(newWidth, newHeight);
}


bool DXEngine::CreateSwapChain()
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


void DXEngine::DrawScene()
{
    if(bInitialized == false)
    {
        return;
    }
    
    check(mDeviceContext != nullptr);

    mViewport.Clear();

    HR(mSwapChain->Present(0,0));
}