
#include "SSDX12.h"
#include "SSDX12VertexBuffer.h"

SSDX12VertexBuffer::SSDX12VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize)
{
	CreateVertexBuffer(device, cmdList, pVertexData, byteSize);
}

void SSDX12VertexBuffer::CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize)
{
	mVertexBufferSize = byteSize;

	check(mVertexBufferSize > 0);

	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVertexBufferSize),
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mResource)));

	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mVertexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mVertexDataUploadBuffer)));


	D3D12_SUBRESOURCE_DATA subResourceData{ pVertexData, mVertexBufferSize, mVertexBufferSize };

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	UpdateSubresources<1>(cmdList, mResource.Get(), mVertexDataUploadBuffer.Get(), 0, 0, 1, &subResourceData);

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mResource.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

	mVertexDataUploadBuffer.Reset();
}