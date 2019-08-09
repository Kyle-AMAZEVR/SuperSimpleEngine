
#include "Core.h"
#include "SSRenderTarget2D.h"
#include "SSEngine.h"
#include "SSMathHelper.h"
#include "DirectXTex.h"

SSRenderTargetTexture2D::SSRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat, bool bGenerateMips, UINT maxMipCount)
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = eFormat;
	mGenerateMips = bGenerateMips;

	if (bGenerateMips)
	{
		check(mWidth == mHeight);

		bool bPowerOfTwo = !(mWidth == 0) && !(mWidth & (mWidth - 1));
		check(bPowerOfTwo);

		mMipLevels = maxMipCount;
		InternalCreate(width, height, eFormat, mMipLevels);
	}
	else
	{
		mMipLevels = 1;
		InternalCreate(width, height, eFormat, 1);
	}
}


void SSRenderTargetTexture2D::InternalCreate(const UINT width, const UINT height, DXGI_FORMAT format, const UINT mipLevels)
{
	D3D11_TEXTURE2D_DESC textureDesc{ 0 };
	textureDesc.Width = mWidth = width;
	textureDesc.Height = mHeight = height;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;
	if (mGenerateMips)
	{
		textureDesc.MiscFlags = D3D11_RESOURCE_MISC_GENERATE_MIPS;
		textureDesc.MipLevels = mipLevels;
	}
	else
	{
		textureDesc.MiscFlags = 0;
		textureDesc.MipLevels = 1;
	}
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.SampleDesc.Count = 1;
	
	textureDesc.ArraySize = 1;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.Format = mTextureFormat = format;

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &mTexturePtr));

	for (UINT i = 0; i < mipLevels; ++i)
	{
		D3D11_RENDER_TARGET_VIEW_DESC renderTargetDesc;
		ZeroMemory(&renderTargetDesc, sizeof(D3D11_RENDER_TARGET_VIEW_DESC));
		renderTargetDesc.Format = textureDesc.Format;
		renderTargetDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
		renderTargetDesc.Texture2D.MipSlice = i;

		HR(SSEngine::Get().GetDevice()->CreateRenderTargetView(mTexturePtr, &renderTargetDesc, &mRenderTargetView[i]));
	}
	
	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	ZeroMemory(&shaderResourceViewDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	
	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MostDetailedMip = 0;
	shaderResourceViewDesc.Texture2D.MipLevels = mipLevels;

	HR(SSEngine::Get().GetDevice()->CreateShaderResourceView(mTexturePtr, &shaderResourceViewDesc, &mShaderResourceView));
}

void SSRenderTargetTexture2D::Destroy()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mShaderResourceView);

	for (UINT i = 0; i < mMipLevels; ++i)
	{
		ReleaseCOM(mRenderTargetView[i]);
	}
}

void SSRenderTargetTexture2D::Resize(const UINT newWidth, const UINT newHeight)
{
	// 
	if (mGenerateMips)
	{
		return;
	}

	if (mWidth != newWidth || mHeight != newHeight)
	{
		Destroy();
		InternalCreate(newWidth, newHeight, mTextureFormat, 1);
	}
}

void SSRenderTargetTexture2D::Clear()
{
	float Color[4]{ 1.0f, 1.0f, 0.0f, 1.0f };

	for (UINT i = 0; i < mMipLevels; ++i)
	{
		SSEngine::Get().GetDeviceContext()->ClearRenderTargetView(mRenderTargetView[i], Color);
	}
}

void SSRenderTargetTexture2D::SaveAsDDSFile(std::wstring filename)
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

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&textureDesc, nullptr, &copiedTexturePtr));

	SSEngine::Get().GetDeviceContext()->CopyResource(copiedTexturePtr, mTexturePtr);

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
			HR(SSEngine::Get().GetDeviceContext()->Map(copiedTexturePtr, subresource, D3D11_MAP_READ, 0, &mapped));
						
			InitD3DDesc(imageList[i]);
			imageList[i].width = mipTextureWidth;
			imageList[i].height = mipTextureHeight;
			imageList[i].pixels = new uint8_t[mapped.RowPitch * mipTextureHeight];
			imageList[i].rowPitch = mapped.RowPitch;
			imageList[i].format = mTextureFormat;
			imageList[i].slicePitch = mapped.RowPitch * mipTextureHeight;

			// copy raw data
			memcpy_s(imageList[i].pixels, mapped.RowPitch * mipTextureHeight, mapped.pData, mapped.RowPitch * mipTextureHeight);

			SSEngine::Get().GetDeviceContext()->Unmap(copiedTexturePtr, subresource);
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
		HR(SSEngine::Get().GetDeviceContext()->Map(copiedTexturePtr, 0, D3D11_MAP_READ, 0, &mapped));

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

		SSEngine::Get().GetDeviceContext()->Unmap(copiedTexturePtr, 0);

		HR(DirectX::SaveToDDSFile(image, 0, filename.c_str()));
		delete[] image.pixels;
	}

	ReleaseCOM(copiedTexturePtr);
}
///////////////////////////

SSDepthRenderTargetTexture2D::SSDepthRenderTargetTexture2D(const UINT width, const UINT height, DXGI_FORMAT eFormat)		
{
	mWidth = width;
	mHeight = height;
	mTextureFormat = eFormat;
	
	InternalCreate(width, height, eFormat);
}

void SSDepthRenderTargetTexture2D::Clear()
{
	SSEngine::Get().GetDeviceContext()->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void SSDepthRenderTargetTexture2D::Destroy()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mDepthStencilView);
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

	HR(SSEngine::Get().GetDevice()->CreateTexture2D(&depthStencilDesc, nullptr, &mTexturePtr));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc{ 0 };	

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	HR(SSEngine::Get().GetDevice()->CreateDepthStencilView(mTexturePtr, &depthStencilViewDesc, &mDepthStencilView));
}



SSGenericRenderTarget::SSGenericRenderTarget(UINT width, UINT height, UINT count, bool bDepthExist, DXGI_FORMAT eFormat, DXGI_FORMAT eDepthFormat)
	: mWidth(width), mHeight(height), mFormat(eFormat), mCount(count)
{
	check(mCount >= 1);
	check(mCount <= 4);

	for (UINT i = 0; i < mCount; ++i)
	{
		mRenderTargetArray[i] = new SSRenderTargetTexture2D(mWidth, mHeight, mFormat);		
	}

	mDepthExist = bDepthExist;

	if (mDepthExist)
	{
		mDepthTarget = new SSDepthRenderTargetTexture2D(mWidth, mHeight);
	}

	// Set the viewport transform.
	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mWidth);
	mViewport.Height = static_cast<float>(mHeight);
	mViewport.MinDepth = 0.0f;
	mViewport.MaxDepth = 1.0f;
}

void SSGenericRenderTarget::SetCurrentRenderTarget()
{
	ID3D11RenderTargetView** renderTargets = new ID3D11RenderTargetView*[mCount];

	for(UINT i = 0; i < mCount; ++i)
	{
		renderTargets[i] = mRenderTargetArray[i]->GetRenderTargetView();
	}

	if(mDepthExist)
	{
		ID3D11DepthStencilView* depthStencil = mDepthTarget->GetDepthStencilView();

		SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(mCount, renderTargets, depthStencil);

		SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
	}
	else
	{
		SSEngine::Get().GetDeviceContext()->OMSetRenderTargets(mCount, renderTargets, nullptr);

		SSEngine::Get().GetDeviceContext()->RSSetViewports(1, &mViewport);
	}
}

void SSGenericRenderTarget::Resize(UINT width, UINT height)
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

	SetCurrentRenderTarget();
}

SSRenderTargetTexture2D* SSGenericRenderTarget::GetOutput(UINT nIndex)
{
	check(nIndex >= 0);
	check(nIndex < mCount);

	if(nIndex < mCount)
	{
		return mRenderTargetArray[nIndex];
	}

	return nullptr;
}

void SSGenericRenderTarget::SaveRTTexture(UINT index, std::wstring filename)
{
	mRenderTargetArray[index]->SaveAsDDSFile(filename);
}

void SSGenericRenderTarget::Clear()
{
	for(UINT i = 0; i < mCount;++i)
	{
		mRenderTargetArray[i]->Clear();
	}

	if (mDepthExist)
	{
		mDepthTarget->Clear();
	}

	
}