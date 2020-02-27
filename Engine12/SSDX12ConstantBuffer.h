#pragma once

#include "SSDX12Resource.h"

class SSDX12_API SSDX12ConstantBuffer : public SSDX12Resource
{
public:
	SSDX12ConstantBuffer() = default;
	SSDX12ConstantBuffer(ID3D12Device* device, const UINT bufferSize = 256);

	// @
	virtual ComPtr<ID3D12Resource> GetResource() const override { return mResource; }
	virtual ID3D12Resource* GetResourcePtr() const override { return mResource.Get(); }
	virtual ComPtr<ID3D12DescriptorHeap> GetDescriptorHeap() const override { return mDescriptorHeap; }
	// @

	UINT GetBufferSize() const { return mCBufferSize; }

protected:
	bool Create(ID3D12Device* device, const UINT bufferSize = 256);

	ComPtr<ID3D12Resource> mResource = nullptr;
	ComPtr<ID3D12DescriptorHeap> mDescriptorHeap = nullptr;
	
	UINT mCBufferSize = 0;
	bool mCreated = false;
	BYTE* mMappedData = nullptr;
};

template<typename T>
class SSDX12TypedConstantBuffer : public SSDX12ConstantBuffer
{
public:
	SSDX12TypedConstantBuffer(ID3D12Device* device)
		: SSDX12ConstantBuffer(device, sizeof(T))
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


