
#include "Core.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSRenderTarget2D::SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT format)
{
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