#pragma once
#include "SSDX12Resource.h"
#include "SSRenderThreadObject.h"

class SSDX12VertexBuffer : public SSRenderThreadResidentObject
{
public:
	SSDX12VertexBuffer() = default;
	SSDX12VertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize, const UINT byteStride);

	// @
	virtual ComPtr<ID3D12Resource> GetResource() const  { return mResource; }
	virtual ID3D12Resource* GetResourcePtr() const  { return mResource.Get(); }	
	// @

	D3D12_VERTEX_BUFFER_VIEW GetVertexBufferView() const;

protected:
	
	ComPtr<ID3D12Resource> mVertexDataUploadBuffer = nullptr;

	ComPtr<ID3D12Resource> mResource = nullptr;
	
	UINT mVertexBufferSize = 0;
	UINT mVertexByteStride = 0;

	void CreateVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const void* pVertexData, const UINT byteSize, const UINT byteStride);
};

template<class T>
class SSDX12TypedVertexBuffer : public SSDX12VertexBuffer
{
public:
	SSDX12TypedVertexBuffer() = default;
	SSDX12TypedVertexBuffer(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::vector<T>& vertexData);
};

template <class T>
SSDX12TypedVertexBuffer<T>::SSDX12TypedVertexBuffer(struct ID3D12Device* device, ID3D12GraphicsCommandList* cmdList, const std::vector<T>& vertexData)
{	
	CreateVertexBuffer(device, cmdList, vertexData.data(), static_cast<UINT>(vertexData.size() * sizeof(T)), sizeof(T));
}