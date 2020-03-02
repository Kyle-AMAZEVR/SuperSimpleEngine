

#include "SSDX12.h"
#include "SSDX12DescriptorHeap.h"

SSDX12CBVSRVUAVDescriptorHeap::SSDX12CBVSRVUAVDescriptorHeap(ID3D12Device* device, UINT numDescriptors)
{
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.NumDescriptors = numDescriptors;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	
	HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)));



	mIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE::D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

CD3DX12_CPU_DESCRIPTOR_HANDLE SSDX12CBVSRVUAVDescriptorHeap::GetBaseDescriptorHandle() const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());	

	return handle;
}


CD3DX12_CPU_DESCRIPTOR_HANDLE SSDX12CBVSRVUAVDescriptorHeap::GetCurrentOffsetHandle() const
{
	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	handle.Offset(mCurrentResourceCount, mIncrementSize);
	return handle;
}

void SSDX12CBVSRVUAVDescriptorHeap::IncrementCurrentResourceCount()
{
	mCurrentResourceCount++;
}