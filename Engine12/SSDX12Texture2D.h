#pragma once

#include "SSDX12Resource.h"
#include "DirectXTex.h"

class SSDX12_API SSDX12Texture2D : public SSDX12Resource 
{
public:
	UINT GetWidth() { return mWidth; }
	UINT GetHeight() { return mHeight; }

	// @
	virtual ComPtr<ID3D12Resource> GetResource() const override { return mTextureResource; }
	virtual ID3D12Resource* GetResourcePtr() const override { return mTextureResource.Get(); }
	virtual ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const override { return mTextureDescriptorHeap; }
	// @	
	
	bool LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, std::wstring filename, bool bSRGB = false);	

protected:

	void CreateDescriptorHeap(ID3D12Device* device);

	UINT mWidth;
	UINT mHeight;
	UINT mMipLevels = 1;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;

	ComPtr<ID3D12Resource> mUploadBuffer;	
	ComPtr<ID3D12Resource> mTextureResource = nullptr;

	ComPtr<ID3D12DescriptorHeap> mTextureDescriptorHeap = nullptr;

	bool LoadInternal(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bSRGB);
};

