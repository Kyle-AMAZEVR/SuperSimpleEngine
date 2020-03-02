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
	// @	

	bool LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, std::wstring filename, bool bSRGB = false);	

protected:

	void CreateShaderResourceView(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle);

	UINT mWidth;
	UINT mHeight;
	UINT mMipLevels = 1;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;

	ComPtr<ID3D12Resource> mUploadBuffer;	
	ComPtr<ID3D12Resource> mTextureResource = nullptr;
	
	bool LoadInternal(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bSRGB);
};

