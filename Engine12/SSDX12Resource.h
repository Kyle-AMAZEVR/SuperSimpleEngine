#pragma once


class SSDX12_API SSDX12Resource
{
public:
	ComPtr<ID3D12Resource> GetResource() const { return mResource; }
	ID3D12Resource* GetResourcePtr() const { return mResource.Get(); }
	
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return mDescriptorHeap; }

protected:
	ComPtr<ID3D12Resource> mResource = nullptr;
	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap = nullptr;
};
