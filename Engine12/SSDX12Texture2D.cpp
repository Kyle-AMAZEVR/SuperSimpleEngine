
#include "SSDX12.h"
#include "SSDX12Texture2D.h"
#include "SSEngine12.h"

bool SSDX12Texture2D::LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, std::wstring filename, bool bSRGB)
{
	DirectX::TexMetadata metaData;
	
	DirectX::ScratchImage image;

	// 1. First Load texture using DXT Lib
	HRESULT result = (DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image));
	if (result != S_OK)
	{
		return false;
	}

	// 2. check dimension
	if(metaData.dimension != DirectX::TEX_DIMENSION_TEXTURE2D)
	{
		return false;
	}

	if(!LoadInternal(device, cmdList, metaData, image, bSRGB))
	{
		return false;
	}	

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
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mTextureResource)));

	const UINT num2DSubresources = 1 * static_cast<UINT>(metaData.mipLevels);
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(mTextureResource.Get(), 0, num2DSubresources);

	// create upload buffer resource
	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUploadBuffer)));
	
	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mTextureResource.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		
	D3D12_SUBRESOURCE_DATA* subresourceData = new D3D12_SUBRESOURCE_DATA[metaData.mipLevels];

	for(int i = 0; i < metaData.mipLevels;++i)
	{
		auto* pLodImage = image.GetImage(i, 0, 0);
		check(pLodImage != nullptr);
		subresourceData[i].RowPitch = pLodImage->rowPitch;
		subresourceData[i].SlicePitch = pLodImage->slicePitch;
		subresourceData[i].pData = pLodImage->pixels;
	}
	
	UpdateSubresources(cmdList, mTextureResource.Get(), mUploadBuffer.Get(), 0, 0, num2DSubresources, subresourceData);

	delete subresourceData;

	cmdList->ResourceBarrier(1, 
		&CD3DX12_RESOURCE_BARRIER::Transition(mTextureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));

	return true;
}

void SSDX12Texture2D::CreateShaderResourceView(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = mTextureFormat;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = mMipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.f;

	device->CreateShaderResourceView(mTextureResource.Get(), &srvDesc, handle);
}

// 
void SSDX12Texture2D::CreateDescriptorHeap(ID3D12Device* device)
{
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc{};
	srvHeapDesc.NumDescriptors = 1;
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HR(device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&mTextureDescriptorHeap)));
}
