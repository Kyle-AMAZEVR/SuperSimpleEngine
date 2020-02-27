#pragma once
#include "SSDX12Resource.h"

class SSDX12VertexBuffer : public SSDX12Resource
{
public:
	SSDX12VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize);

protected:
	ComPtr<ID3D12Resource> mVertexDataUploadBuffer = nullptr;
	
	UINT mVertexBufferSize = 0;

	void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize);
};

template<class T>
class SSDX12TypedVertexBuffer : public SSDX12VertexBuffer
{
public:
	SSDX12TypedVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::vector<T>& vertexData);
};

template <class T>
SSDX12TypedVertexBuffer<T>::SSDX12TypedVertexBuffer(struct ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::vector<T>& vertexData)
{	
	CreateVertexBuffer(device, cmdList, vertexData.data(), vertexData.size() * sizeof(T));
}