
#include "SSRendererModulePCH.h"
#include "SSDX11RenderTarget.h"
#include "SSDX11Renderer.h"
#include "SSMathHelper.h"
#include "DirectXTex.h"
#include "SSDX11Device.h"

SSDX11RenderTargetTexture2D::SSDX11RenderTargetTexture2D(
	ID3D11Texture2D* InTexture,
	ID3D11ShaderResourceView* InSRV,
	std::vector<ID3D11RenderTargetView*>& InRTArray,
	const UINT width,
	const UINT height,
	DXGI_FORMAT eFormat,
	bool bGeneratedMips,
	UINT maxMipCount)
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = eFormat;
	mGenerateMips = bGeneratedMips;

	if (bGeneratedMips)
	{
		check(mWidth == mHeight);

		bool bPowerOfTwo = !(mWidth == 0) && !(mWidth & (mWidth - 1));
		check(bPowerOfTwo);

		mMipLevels = maxMipCount;
	}
	else
	{
		mMipLevels = 1;		
	}

	mTexturePtr = InTexture;
	mShaderResourceView = InSRV;

	mRenderTargetView.resize(mMipLevels);

	for (unsigned int i = 0; i < mMipLevels; ++i)
	{
		mRenderTargetView[i] = InRTArray[i];
		mRenderTargetView[i]->AddRef();
	}
}


SSDX11RenderTargetTexture2D::~SSDX11RenderTargetTexture2D()
{
	Destroy();
}


void SSDX11RenderTargetTexture2D::Destroy()
{
	mTexturePtr.Reset();
	mShaderResourceView.Reset();	

	for (UINT i = 0; i < mMipLevels; ++i)
	{
		mRenderTargetView[i]->Release();
	}	
}

void SSDX11RenderTargetTexture2D::Resize(const UINT newWidth, const UINT newHeight)
{
	// 
	if (mGenerateMips)
	{
		return;
	}

	if (mWidth != newWidth || mHeight != newHeight)
	{
		GetDX11Device()->ResizeRenderTargetTexture2D(this, newWidth, newHeight);
	}
}


void SSDX11RenderTargetTexture2D::Clear(ID3D11DeviceContext* deviceContext)
{
	check(deviceContext);

	float Color[4]{ 0.0f, 1.0f, 0.0f, 1.0f };

	for (UINT i = 0; i < mMipLevels; ++i)
	{
		deviceContext->ClearRenderTargetView(mRenderTargetView[i], Color);
	}
}

void SSDX11RenderTargetTexture2D::SaveAsDDSFile(std::wstring filename)
{

	// @create staging texture for copy
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth;
	textureDesc.Height = mHeight;
	textureDesc.BindFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.MipLevels = mMipLevels;
	textureDesc.Usage = D3D11_USAGE_STAGING;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
	textureDesc.Format = mTextureFormat;
	ID3D11Texture2D* copiedTexturePtr = nullptr;

	HR(SSDX11Renderer::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &copiedTexturePtr));

	SSDX11Renderer::Get().GetImmediateDeviceContext()->CopyResource(copiedTexturePtr, mTexturePtr.Get());

	// copy resource
	if (mGenerateMips)
	{
		DirectX::Image* imageList = new DirectX::Image[mMipLevels];

		for (UINT i = 0; i < mMipLevels; ++i)
		{
			const UINT mipTextureHeight = mWidth / static_cast<UINT>(std::pow(2, i));
			const UINT mipTextureWidth = mHeight / static_cast<UINT>(std::pow(2, i));

			UINT subresource = D3D11CalcSubresource(i, 0, mMipLevels);

			D3D11_MAPPED_SUBRESOURCE mapped;
			HR(SSDX11Renderer::Get().GetImmediateDeviceContext()->Map(copiedTexturePtr, subresource, D3D11_MAP_READ, 0, &mapped));
						
			InitD3DDesc(imageList[i]);
			imageList[i].width = mipTextureWidth;
			imageList[i].height = mipTextureHeight;
			imageList[i].pixels = new uint8_t[mapped.RowPitch * mipTextureHeight];
			imageList[i].rowPitch = mapped.RowPitch;
			imageList[i].format = mTextureFormat;
			imageList[i].slicePitch = mapped.RowPitch * mipTextureHeight;

			// copy raw data
			memcpy_s(imageList[i].pixels, mapped.RowPitch * mipTextureHeight, mapped.pData, mapped.RowPitch * mipTextureHeight);

			SSDX11Renderer::Get().GetImmediateDeviceContext()->Unmap(copiedTexturePtr, subresource);
		}

		TexMetadata metaData;
		InitD3DDesc(metaData);
		metaData.arraySize = 1;
		metaData.dimension = TEX_DIMENSION_TEXTURE2D;
		metaData.format = mTextureFormat;
		metaData.width = mWidth;
		metaData.height = mHeight;

		HR(DirectX::SaveToDDSFile(imageList, mMipLevels, metaData, 0, filename.c_str()));
		
		for (UINT i = 0; i < mMipLevels; ++i)
		{
			delete[] imageList[i].pixels;
		}
	}
	else
	{
		D3D11_MAPPED_SUBRESOURCE mapped;
		HR(SSDX11Renderer::Get().GetImmediateDeviceContext()->Map(copiedTexturePtr, 0, D3D11_MAP_READ, 0, &mapped));

		DirectX::Image image;
		InitD3DDesc(image);
		image.width = mWidth;
		image.height = mHeight;
		image.pixels = new uint8_t[mapped.RowPitch * mHeight];
		image.rowPitch = mapped.RowPitch;
		image.format = mTextureFormat;
		image.slicePitch = mapped.RowPitch * mHeight;

		// copy raw data
		memcpy_s(image.pixels, mapped.RowPitch * mHeight, mapped.pData, mapped.RowPitch * mHeight);

		SSDX11Renderer::Get().GetImmediateDeviceContext()->Unmap(copiedTexturePtr, 0);

		HR(DirectX::SaveToDDSFile(image, 0, filename.c_str()));
		delete[] image.pixels;
	}

	ReleaseCOM(copiedTexturePtr);
}
///////////////////////////

SSDepthRenderTargetTexture2D::SSDepthRenderTargetTexture2D(
	const UINT InWidth, 
	const UINT InHeight, 
	DXGI_FORMAT InFormat, 
	ID3D11Texture2D* InTexture, 
	ID3D11DepthStencilView* InDepthStencilView)
{
	mWidth = InWidth;
	mHeight = InHeight;
	mTextureFormat = InFormat;
	mTexturePtr = InTexture;
	mDepthStencilView = InDepthStencilView;
}

void SSDepthRenderTargetTexture2D::Clear(ID3D11DeviceContext* deviceContext)
{
	check(deviceContext);

	deviceContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

 SSDepthRenderTargetTexture2D::~SSDepthRenderTargetTexture2D()
{
	Destroy();	
}

void SSDepthRenderTargetTexture2D::Destroy()
{
	mTexturePtr.Reset();
	mDepthStencilView.Reset();	
}

void SSDepthRenderTargetTexture2D::Resize(const UINT newWidth, const UINT newHeight)
{
	Destroy();
	InternalCreate(newWidth, newHeight, mTextureFormat);
}

void SSDepthRenderTargetTexture2D::InternalCreate(const UINT newWidth, const UINT height, DXGI_FORMAT format)
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

	HR(SSDX11Renderer::Get().GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &mTexturePtr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ 0 };	

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(SSDX11Renderer::Get().GetDevice()->CreateDepthStencilView(mTexturePtr.Get(), &depthStencilViewDesc, &mDepthStencilView));
}



SSDX11RenderTarget::SSDX11RenderTarget(UINT width, UINT height, UINT count, bool bDepthExist, DXGI_FORMAT eFormat, DXGI_FORMAT eDepthFormat)
	: mWidth(width), mHeight(height), mFormat(eFormat), mCount(count)
{
	check(mCount >= 1);
	check(mCount <= 4);

	for (UINT i = 0; i < mCount; ++i)
	{		
		mRenderTargetArray[i] = GetDX11Device()->CreateRenderTargetTexture2D(mWidth, mHeight, mFormat);	
	}

	mDepthExist = bDepthExist;

	if (mDepthExist)
	{
		mDepthTarget = GetDX11Device()->CreateDepthRenderTargetTexture2D(mWidth, mHeight);
	}

	// Set the viewport transform.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;	
}

ID3D11RenderTargetView* SSDX11RenderTarget::GetRenderTargetView(UINT nIndex) const
{
	return mRenderTargetArray[nIndex]->GetRenderTargetView();
}

ID3D11DepthStencilView* SSDX11RenderTarget::GetDepthStencilView() const
{
	if (mDepthExist && mDepthTarget)
	{
		return mDepthTarget->GetDepthStencilView();
	}

	return nullptr;
}

void SSDX11RenderTarget::SetCurrentRenderTarget(SSDX11Device* device)
{
	for (UINT i = 0; i < mCount; ++i)
	{
		mRenderTargetViews[i] = mRenderTargetArray[i]->GetRenderTargetView();
	}

	if (mDepthExist)
	{
		ID3D11DepthStencilView* depthStencil = mDepthTarget->GetDepthStencilView();

		device->GetDeviceContext()->OMSetRenderTargets(mCount, mRenderTargetViews, depthStencil);

		device->GetDeviceContext()->RSSetViewports(1, &mViewport);
	}
	else
	{		
		device->GetDeviceContext()->OMSetRenderTargets(mCount, mRenderTargetViews, nullptr);

		device->GetDeviceContext()->RSSetViewports(1, &mViewport);
	}
}

void SSDX11RenderTarget::Resize(UINT width, UINT height)
{
	mWidth = width;
	mHeight = height;

	for(UINT i = 0; i < mCount; ++i)
	{
		mRenderTargetArray[i]->Resize(mWidth, mHeight);
	}	

	if (mDepthExist)
	{
		mDepthTarget->Resize(mWidth, mHeight);
	}

	// Set the viewport transform.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;	
}

SSDX11RenderTargetTexture2D* SSDX11RenderTarget::GetOutput(UINT nIndex)
{
	check(nIndex >= 0);
	check(nIndex < mCount);

	if(nIndex < mCount)
	{
		return mRenderTargetArray[nIndex];
	}

	return nullptr;
}

void SSDX11RenderTarget::Destroy()
{
	for(UINT i = 0; i < mCount; ++i)
	{
		if(mRenderTargetArray[i])
		{
			mRenderTargetArray[i]->Destroy();
		}
	}

	if(mDepthTarget != nullptr)
	{
		mDepthTarget->Destroy();
	}
}

void SSDX11RenderTarget::SaveRTTexture(UINT index, std::wstring filename)
{
	mRenderTargetArray[index]->SaveAsDDSFile(filename);
}

void SSDX11RenderTarget::Clear(SSDX11Device* device)
{
	for(UINT i = 0; i < mCount;++i)
	{
		mRenderTargetArray[i]->Clear(device->GetDeviceContext());
	}

	if (mDepthExist)
	{
		mDepthTarget->Clear(device->GetDeviceContext());
	}

	
}