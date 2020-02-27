
#include "SSDX12.h"
#include "SSDX12IndexBuffer.h"

D3D12_INDEX_BUFFER_VIEW SSDX12IndexBuffer::GetIndexBufferView() const
{
	D3D12_INDEX_BUFFER_VIEW indexBufferView;
	indexBufferView.BufferLocation = mIndexBuffer->GetGPUVirtualAddress();
	indexBufferView.Format = mIndexBufferFormat;
	indexBufferView.SizeInBytes = mIndexBufferSize;

	return indexBufferView;
}

void SSDX12IndexBuffer::CreateIndexBuffer(ID3D12Device * device, ID3D12GraphicsCommandList * cmdList, const std::vector<UINT>& indexData)
{
	mIndexBufferSize = sizeof(UINT) * static_cast<UINT>(indexData.size());

	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mIndexBufferSize),
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&mIndexBuffer)));

	HR(device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(mIndexBufferSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mIndexDataUploadBuffer)));


	D3D12_SUBRESOURCE_DATA subResourceData{ indexData.data(), mIndexBufferSize, mIndexBufferSize };

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mIndexBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	UpdateSubresources<1>(cmdList, mIndexBuffer.Get(), mIndexDataUploadBuffer.Get(), 0, 0, 1, &subResourceData);

	cmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mIndexBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_COMMON));

	mIndexDataUploadBuffer.Reset();	
}
