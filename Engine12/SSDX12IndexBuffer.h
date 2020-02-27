#pragma once

#include "SSDX12Resource.h"

class SSDX12IndexBuffer : public SSDX12Resource
{
public:

	
	virtual ComPtr<ID3D12Resource> GetResource() const { return mIndexBuffer; }
	virtual ID3D12Resource* GetResourcePtr() const { return mIndexBuffer.Get(); }
	virtual ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return nullptr; }

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const;
protected:

	void CreateIndexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::vector<UINT>& indexData);

	ComPtr<ID3D12Resource> mIndexBuffer;
	ComPtr<ID3D12Resource> mIndexDataUploadBuffer;

	UINT mIndexBufferSize = 0;
	DXGI_FORMAT mIndexBufferFormat = DXGI_FORMAT_R32_UINT;
};
