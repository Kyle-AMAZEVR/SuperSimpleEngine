
#include "Core.h"
#include "Util.h"
#include "DXEngine.h"
#include "DXShader.h"
#include "DXVertexTypes.h"
#include "DXVertexElementDeclaration.h"

bool DXEngine::bInitialized = false;

bool DXEngine::Initialize(HWND windowHandle)
{
    mWindowHandle = windowHandle;
    CreateDevice();
    CreateSwapChain();
    OnWindowResize(1024,768);   
    bInitialized = true;


    TestCompileShader();
    TestCreateResources();

    return true;
}

void DXEngine::TestCreateResources()
{   
    mTestVertexBuffer = std::make_shared<DXVertexBuffer>();
    mTestIndexBuffer = std::make_shared<DXIndexBuffer>();

    std::vector<VT_PositionColor> VertexArray =
    {
        
        {DirectX::XMFLOAT4(-1, -1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},
        {DirectX::XMFLOAT4(-1,  1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},
        {DirectX::XMFLOAT4( 1,  1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},

        {DirectX::XMFLOAT4(-1, -1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},
        {DirectX::XMFLOAT4( 1,  1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},
        {DirectX::XMFLOAT4( 1, -1, -1, 1), DirectX::XMFLOAT4(1,0,0,1)},
        
    };

    std::vector<UINT> IndexArray = 
    {
        0,1,2,3,4,5
    };

    mTestVertexBuffer->SetVertexData<VT_PositionColor>(VertexArray);
    mTestIndexBuffer->SetIndexBufferData(IndexArray);
}

void DXEngine::TestCompileShader()
{    
    mTestVertexShader = std::make_shared<DXVertexShader>();
    mTestVertexShader->CompileFromFile(L"./Shader/BasicShader.vs");
    
    mTestPixelShader = std::make_shared<DXPixelShader>();
    mTestPixelShader->CompileFromFile(L"./Shader/BasicShader.ps");
    
    mDebug->ReportLiveDeviceObjects(D3D11_RLDO_DETAIL);
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


void DXEngine::DrawScene()
{
    if(bInitialized == false)
    {
        return;
    }
    
    check(mDeviceContext != nullptr);

    mViewport.Clear();

    UINT stride = sizeof(VT_PositionColor);
    UINT offset = 0;

    mDeviceContext->IASetInputLayout(mTestVertexShader->GetInputLayout());

    mDeviceContext->VSSetShader(mTestVertexShader->GetShader(), nullptr, 0);    

    

    mDeviceContext->IASetVertexBuffers(0, 1, &mTestVertexBuffer->GetBufferPointerRef(), &stride, &offset);

    

    mDeviceContext->IASetIndexBuffer(mTestIndexBuffer->GetBufferPointer(), DXGI_FORMAT_R32_UINT, 0);

    mDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    

    HR(mSwapChain->Present(0,0));
}