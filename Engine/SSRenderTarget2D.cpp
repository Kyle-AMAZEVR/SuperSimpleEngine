
#include "Core.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"

SSRenderTarget2D::SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT format)
{
	InternalCreate(width, height, format);
}
void SSRenderTarget2D::InternalCreate(const UINT width, const UINT height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = width;
	description.Height = mHeight = height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DEFAULT;
	description.SampleDesc.Count = 1;
	description.MipLevels = description.ArraySize = 1;
	description.CPUAccessFlags = 0;
	description.Format = mTextureFormat = format;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));
	//HR(DXEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, nullptr, &mShaderResourceView));
}

void SSRenderTarget2D::Release()
{
	if (mTexturePtr != nullptr)
	{
		mTexturePtr->Release();
		mTexturePtr = nullptr;
	}
	if (mShaderResourceView != nullptr)
	{
		mShaderResourceView->Release();
		mShaderResourceView = nullptr;
	}
}

void SSRenderTarget2D::Resize(const UINT newWidth, const UINT newHeight)
{
	if (mWidth != newWidth || mHeight != newHeight)
	{
		Release();
		InternalCreate(newWidth, newHeight, mTextureFormat);
	}
}