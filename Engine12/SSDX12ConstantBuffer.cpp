
#include "SSDX12.h"
#include "SSDX12ConstantBuffer.h"
#include "d3dx12.h"

SSDX12ConstantBuffer::SSDX12ConstantBuffer(ID3D12Device* device, const UINT bufferSize)
{
	check(Create(device, bufferSize) == true);
}

bool SSDX12ConstantBuffer::Create(ID3D12Device* device, const UINT bufferSize)
{
	// create descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc;
	heapDesc.NumDescriptors = 1;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;

	HR(device->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mDescriptorHeap)));

	mCBufferSize = (bufferSize + 255) & (~255);

	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE::D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE, 
		&CD3DX12_RESOURCE_DESC::Buffer(mCBufferSize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&mResource)));

	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.BufferLocation = mResource->GetGPUVirtualAddress();
	desc.SizeInBytes = mCBufferSize;

	// create descriptor
	// CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDescriptorHeap->GetCPUDescriptorHandleForHeapStart());
	// device->CreateConstantBufferView(&desc, handle);

	mCreated = true;

	return true;
}

void SSDX12ConstantBuffer::CreateConstantBufferView(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC desc;
	desc.BufferLocation = mResource->GetGPUVirtualAddress();
	desc.SizeInBytes = mCBufferSize;

	device->CreateConstantBufferView(&desc, handle);
}