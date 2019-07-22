
#include "Core.h"
#include "SSRenderTarget2D.h"
#include "DXEngine.h"

SSRenderTarget2D::SSRenderTarget2D(const UINT width, const UINT height, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC description;
	description.Width = mWidth = width;
	description.Height = mHeight = height;
	description.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	description.MiscFlags = 0;
	description.Usage = D3D11_USAGE_DYNAMIC;
	description.SampleDesc.Count = 1;
	description.MipLevels = description.ArraySize = 1;
	description.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	HR(DXEngine::Get().GetDevice()->CreateTexture2D(&description, nullptr, &mTexturePtr));
	HR(DXEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, nullptr, &mShaderResourceView));
}