
#include "SSDX12.h"
#include "SSDX12Texture2D.h"

bool SSDX12Texture2D::LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, std::wstring filename, bool bSRGB)
{
	DirectX::TexMetadata metaData;
	
	DirectX::ScratchImage image;

	HRESULT result = (DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));
	if (result != S_OK)
	{
		return false;
	}

	check(metaData.dimension == DirectX::TEX_DIMENSION_TEXTURE2D);

	LoadInternal(device, cmdList, metaData, image, bSRGB);

	return true;
}

bool LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, std::string filename, bool bSRGB)
{
	

	return true;
}


bool SSDX12Texture2D::LoadInternal(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bSRGB)
{
	mWidth = static_cast<UINT>(metaData.width);
	mHeight = static_cast<UINT>(metaData.height);
	mMipLevels = static_cast<UINT>(metaData.mipLevels);
	
	if (bSRGB)
	{
		mTextureFormat = DirectX::MakeSRGB(metaData.format);
	}
	else
	{
		mTextureFormat = metaData.format;
	}

	CD3DX12_RESOURCE_DESC textureDesc = CD3DX12_RESOURCE_DESC::Tex2D(mTextureFormat, mWidth, mHeight, static_cast<UINT16>(metaData.arraySize), static_cast<UINT16>(metaData.mipLevels));	

	// create texture resource
	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&textureDesc,
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mResource)));

	const UINT num2DSubresources = 1 * static_cast<UINT>(metaData.mipLevels);
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(mResource.Get(), 0, num2DSubresources);

	// create upload buffer resource
	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mResource)));	
	
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		
	D3D12_SUBRESOURCE_DATA* subresourceData = new D3D12_SUBRESOURCE_DATA[metaData.mipLevels];

	for(int i = 0; i < metaData.mipLevels;++i)
	{
		auto* pLodImage = image.GetImage(i, 0, 0);
		check(pLodImage != nullptr);
		subresourceData[i].RowPitch = pLodImage->rowPitch;
		subresourceData[i].SlicePitch = pLodImage->slicePitch;
		subresourceData[i].pData = pLodImage->pixels;
	}
	
	UpdateSubresources(cmdList, mResource.Get(), mUploadBuffer.Get(), 0, 0, num2DSubresources, subresourceData);

	delete subresourceData;

	cmdList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));


	return true;
}