#pragma once

using Microsoft::WRL::ComPtr;

class SSMeshGeometry
{
public:

	ComPtr<ID3D12Resource> mVertexBufferGPU = nullptr;
	ComPtr<ID3D12Resource> mIndexBufferGPU = nullptr;

	ComPtr<ID3D12Resource> mVertexBufferUploader = nullptr;
	ComPtr<ID3D12Resource> mIndexBufferUploader = nullptr;

	UINT mVertexByteStride = 0;
	UINT mVertexBufferByteSize = 0;

	DXGI_FORMAT mIndexBufferFormat = DXGI_FORMAT_R16_UINT;
	UINT mIndexBufferByteSize = 0;
	UINT mIndexCount = 0;

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const
	{
		D3D12_VERTEX_BUFFER_VIEW VBV;
		VBV.BufferLocation = mVertexBufferGPU->GetGPUVirtualAddress();
		VBV.StrideInBytes = mVertexByteStride;
		VBV.SizeInBytes = mVertexBufferByteSize;
		return VBV;
	}

	D3D12_INDEX_BUFFER_VIEW GetIndexBufferView() const
	{
		D3D12_INDEX_BUFFER_VIEW IBV;
		IBV.BufferLocation = mIndexBufferGPU->GetGPUVirtualAddress();
		IBV.Format = mIndexBufferFormat;
		IBV.SizeInBytes = mIndexBufferByteSize;

		return IBV;
	}


};
