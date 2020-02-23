#include "Core.h"
#include "SSEngine.h"
#include "SSViewport.h"
#include "CameraManager.h"

void SSViewport::Clear(ID3D11DeviceContext* deviceContext)
{
	float Color[4]{ 0.0f, 1.0f, 0.0f, 1.0f };

	check(deviceContext);

	if (deviceContext != nullptr)
	{
		deviceContext->ClearRenderTargetView(mRenderTargetView.Get(), Color);
		deviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	}
}

void SSViewport::SetCurrentRenderTarget(ID3D11DeviceContext* deviceContext)
{
	check(deviceContext != nullptr);
	// Bind the render target view and depth/stencil view to the pipeline.
	deviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());

	// Set the viewport transform.
	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(mWidth);
	mScreenViewport.Height = static_cast<float>(mHeight);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	deviceContext->RSSetViewports(1, &mScreenViewport);
}


void SSViewport::Resize(UINT newWidth, UINT newHeight)
{
    auto* dxDevice = SSDX11Engine::Get().GetDevice();
    auto* dxDeviceContext = SSDX11Engine::Get().GetImmediateDeviceContext();
    auto* dxSwapChain = SSDX11Engine::Get().GetSwapChain();
    
	check(dxDevice != nullptr);
	check(dxDeviceContext != nullptr);
	check(dxSwapChain != nullptr);

    UINT msaaQuality;

    HR(dxDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R16G16B16A16_FLOAT, 4, &msaaQuality));    

    check ( newWidth > 0 && newHeight > 0 );
    mWidth = newWidth;
    mHeight = newHeight;

	// Release the old views, as they hold references to the buffers we
	// will be destroying.  Also release the old depth/stencil buffer.
	mRenderTargetView.Reset();
	mDepthStencilView.Reset();
	mDepthStencilBuffer.Reset();	


	// Resize the swap chain and recreate the render target view.
	HR(dxSwapChain->ResizeBuffers(1, mWidth, mHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, 0));
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

	SetCurrentRenderTarget(dxDeviceContext);

	SSCameraManager::Get().SetCurrentCameraAspectRatio(static_cast<float>(mWidth) / static_cast<float>(mHeight));
}