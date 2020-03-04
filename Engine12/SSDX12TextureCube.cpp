
#include "SSDX12.h"
#include "SSDX12TextureCube.h"
#include "DirectXTex.h"


bool SSDX12TextureCube::LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, std::wstring filename, bool bSRGB)
{
	DirectX::TexMetadata metaData;
	DirectX::ScratchImage image;

	HRESULT result = DirectX::LoadFromDDSFile(filename.c_str(), DirectX::DDS_FLAGS::DDS_FLAGS_NONE, &metaData, image);

	if (result != S_OK)
	{
		return false;
	}

	if(metaData.dimension != DirectX::TEX_DIMENSION_TEXTURE2D)
	{
		return false;
	}

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

	const UINT num2DSubResources = metaData.arraySize * static_cast<UINT>(metaData.mipLevels);
	const UINT64 uploadBufferSize = GetRequiredIntermediateSize(mTextureResource.Get(), 0, num2DSubResources);

	// create upload buffer resource
	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mUploadBuffer)));

	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mTextureResource.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	D3D12_SUBRESOURCE_DATA* subresourceData = new D3D12_SUBRESOURCE_DATA[metaData.arraySize * metaData.mipLevels];

	for (UINT face = 0; face < metaData.arraySize; ++face)
	{
		for (UINT mipLevel = 0; mipLevel < metaData.mipLevels; ++mipLevel)
		{
			auto* pLodImage = image.GetImage(mipLevel, face, 0);
			check(pLodImage != nullptr);
			auto dstSubresource =D3D12CalcSubresource(mipLevel, face, 0, metaData.mipLevels, metaData.arraySize);
			subresourceData[dstSubresource].RowPitch = pLodImage->rowPitch;
			subresourceData[dstSubresource].SlicePitch = pLodImage->slicePitch;
			subresourceData[dstSubresource].pData = pLodImage->pixels;
		}
	}

	UpdateSubresources(cmdList, mTextureResource.Get(), mUploadBuffer.Get(), 0, 0, num2DSubResources, subresourceData);

	delete subresourceData;

	cmdList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(mTextureResource.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE));


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));
	srvDesc.Format = mTextureFormat;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = mMipLevels;
	srvDesc.Texture2D.ResourceMinLODClamp = 0.f;

	device->CreateShaderResourceView(mTextureResource.Get(), &srvDesc, handle);

	return true;
}