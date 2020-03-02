

#pragma once
#include <d3d12.h>


class SSDX12CBVSRVUAVDescriptorHeap
{
	
public:
	SSDX12CBVSRVUAVDescriptorHeap(ID3D12Device* device, UINT numDescriptors);

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetBaseDescriptorHandle() const;

	CD3DX12_CPU_DESCRIPTOR_HANDLE GetCurrentOffsetHandle() const;

	void IncrementCurrentResourceCount();

protected:

	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap;
	UINT mCurrentResourceCount = 0;
	UINT mIncrementSize = 0;
};
