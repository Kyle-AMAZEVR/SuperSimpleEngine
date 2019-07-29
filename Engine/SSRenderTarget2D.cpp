
#include "Core.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSRenderTarget2D::SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT format)
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = format;
	InternalCreate(width, height, format);
}
void SSRenderTarget2D::InternalCreate(const UINT width, const UINT height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth = width;
	textureDesc.Height = mHeight = height;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.MipLevels = textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = mTextureFormat = format;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &mTexturePtr));

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
	ZeroMemory(&renderTargetDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));

	renderTargetDesc.Format = textureDesc.Format;
	renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	renderTargetDesc.Texture2D.MipSlice = 0;

	HR(SSEngine::Get().GetDevice()->CreateRenderTargetView(mTexturePtr, &renderTargetDesc, &mRenderTargetView));

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &shaderResourceViewDesc, &mShaderResourceView));
}

void SSRenderTarget2D::Destroy()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mShaderResourceView);
	ReleaseCOM(mRenderTargetView);
}

void SSRenderTarget2D::Resize(const UINT newWidth, const UINT newHeight)
{
	if (mWidth != newWidth || mHeight != newHeight)
	{
		Destroy();
		InternalCreate(newWidth, newHeight, mTextureFormat);
	}
}

void SSRenderTarget2D::Clear()
{
	float Color[4]{ 1.0f, 1.0f, 1.0f, 1.0f };
	
	SSEngine::Get().GetDeviceContext()->ClearRenderTargetView(mRenderTargetView, Color);
}

///////////////////////////

SSDepthRenderTarget2D::SSDepthRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT eFormat)		
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = eFormat;
	
	InternalCreate(width, height, eFormat);
}

void SSDepthRenderTarget2D::Clear()
{
	SSEngine::Get().GetDeviceContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SSDepthRenderTarget2D::Destroy()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mDepthStencilView);
}

void SSDepthRenderTarget2D::Resize(const UINT newWidth, const UINT newHeight)
{
	Destroy();
	InternalCreate(newWidth, newHeight, mTextureFormat);
}

void SSDepthRenderTarget2D::InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC depthStencilDesc;

	depthStencilDesc.Width = mWidth = newWidth;
	depthStencilDesc.Height = mHeight =height;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = format;

	// Use 4X MSAA? --must match swap chain MSAA values.
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;

	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &mTexturePtr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ 0 };	

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(SSEngine::Get().GetDevice()->CreateDepthStencilView(mTexturePtr, &depthStencilViewDesc, &mDepthStencilView));
}