#pragma once
#include "SSDX12Resource.h"

class SSDX12TextureCube : public SSDX12Resource
{
public:
	bool LoadFromDDSFile(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, std::wstring filename, bool bSRGB=false);

protected:

	UINT mWidth = 0;
	UINT mHeight = 0;
	UINT mMipLevels = 1;
	DXGI_FORMAT mTextureFormat = DXGI_FORMAT_UNKNOWN;

	ComPtr<ID3D12Resource> mUploadBuffer;
	ComPtr<ID3D12Resource> mTextureResource = nullptr;
};
