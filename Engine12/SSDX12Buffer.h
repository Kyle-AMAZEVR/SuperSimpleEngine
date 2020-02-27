#pragma once


class SSDX12Resource
{
public:
	ComPtr<ID3D12Resource> GetResource() const { return mBuffer; }
	ID3D12Resource* GetResourcePtr() const { return mBuffer.Get(); }
	
	ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const { return mDescriptorHeap; }

protected:
	ComPtr<ID3D12Resource> mBuffer = nullptr;
	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap = nullptr;
};
