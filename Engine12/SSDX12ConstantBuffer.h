#pragma once

#include "SSDX12Resource.h"

class SSDX12_API SSDX12ConstantBuffer : public SSDX12Resource
{
public:
	SSDX12ConstantBuffer() = default;
	SSDX12ConstantBuffer(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const UINT bufferSize = 256);

	// @
	virtual ComPtr<ID3D12Resource> GetResource() const override { return mResource; }
	virtual ID3D12Resource* GetResourcePtr() const override { return mResource.Get(); }	
	// @

	UINT GetBufferSize() const { return mCBufferSize; }	

protected:
	bool Create(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle, const UINT bufferSize = 256);
	void CreateConstantBufferView(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle);

	ComPtr<ID3D12Resource> mResource = nullptr;	
	
	UINT mCBufferSize = 0;
	bool mCreated = false;
	BYTE* mMappedData = nullptr;
};

template<typename T>
class SSDX12TypedConstantBuffer : public SSDX12ConstantBuffer
{
public:
	SSDX12TypedConstantBuffer(ID3D12Device* device, CD3DX12_CPU_DESCRIPTOR_HANDLE handle)
		: SSDX12ConstantBuffer(device, handle, sizeof(T))
	{
	}

	void WriteData(const T& data);
protected:
};

template <typename T>
void SSDX12TypedConstantBuffer<T>::WriteData(const T& data)
{
	if (mResource)
	{
		mResource->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));

		memcpy(mMappedData, &data, sizeof(T));

		mResource->Unmap(0, nullptr);
	}
}


