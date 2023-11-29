#include "SSRendererModulePCH.h"
#include "SSDX11Renderer.h"
#include "SSDX11Viewport.h"
#include "SSCameraManager.h"

void SSDX11Viewport::Clear(SSDX11Device* device)
{
	float Color[4]{ 1, 0, 0, 1.0f };

	SSDX11Device* DX11Device = static_cast<SSDX11Device*>(device);
	
	DX11Device->GetDeviceContext()->ClearRenderTargetView(mRenderTargetView.Get(), Color);
	DX11Device->GetDeviceContext()->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);	
}



void SSDX11Viewport::SetCurrentRenderTarget(SSDX11Device* device)
{
	check(device != nullptr);

	SSDX11Device* dx11Device = static_cast<SSDX11Device*>(device);
	
	// Set the viewport transform.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mWidth);
	mScreenViewport.Height = static_cast<float>(mHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	// Bind the render target view and depth/stencil view to the pipeline.
	dx11Device->GetDeviceContext()->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	dx11Device->GetDeviceContext()->RSSetViewports(1, &mScreenViewport);
}


void SSDX11Viewport::Resize(SSDX11Device* device, UINT newWidth, UINT newHeight)
{
	SSDX11Device* dx11Device = static_cast<SSDX11Device*>(device);

	auto* dxDevice = dx11Device->GetDevice();
	auto* dxDeviceContext = dx11Device->GetDeviceContext();
	auto* dxSwapChain = dx11Device->GetSwapChain();

	check(dxDevice != nullptr);
	check(dxDeviceContext != nullptr);
	check(dxSwapChain != nullptr);

    UINT msaaQuality;

    HR(dxDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &msaaQuality));

    check ( newWidth > 0 && newHeight > 0 );
    mWidth = newWidth;
    mHeight = newHeight;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	mRenderTargetView.Reset();
	mDepthStencilView.Reset();
	mDepthStencilBuffer.Reset();	


	// Resize the swap chain and recreate the render target view.
	HR(dxSwapChain->ResizeBuffers(0, mWidth, mHeight, DXGI_FORMAT_R8G8B8A8_UNORM, 0));
	ID3D11Texture2D* backBuffer;
	HR(dxSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer)));
	HR(dxDevice->CreateRenderTargetView(backBuffer, 0, &mRenderTargetView));
	ReleaseCOM(backBuffer);

	// Create the depth/stencil buffer and view.

	D3D11_TEXTURE2D_DESC depthStencilDesc;
	
	depthStencilDesc.Width     = mWidth;
	depthStencilDesc.Height    = mHeight;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format    = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// Use 4X MSAA? --must match swap chain MSAA values.
    depthStencilDesc.SampleDesc.Count   = 4;
    depthStencilDesc.SampleDesc.Quality = msaaQuality - 1;

	depthStencilDesc.Usage          = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags      = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0; 
	depthStencilDesc.MiscFlags      = 0;

	HR(dxDevice->CreateTexture2D(&depthStencilDesc, 0, &mDepthStencilBuffer));
	HR(dxDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), 0, &mDepthStencilView));

	SetCurrentRenderTarget(device);

	SSCameraManager::Get().SetCurrentCameraAspectRatio(static_cast<float>(mWidth) / static_cast<float>(mHeight));
}