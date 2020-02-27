#pragma once


class SSDX12_API SSDX12Resource
{
public:
	virtual ComPtr<ID3D12Resource> GetResource() const = 0;
	virtual ID3D12Resource* GetResourcePtr() const = 0;	
	virtual ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const = 0;
	
};
