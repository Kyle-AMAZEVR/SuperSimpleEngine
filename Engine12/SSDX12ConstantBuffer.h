#pragma once

#include "SSDX12Buffer.h"


class SSDX12ConstantBuffer : public SSDX12Resource
{
public:
	SSDX12ConstantBuffer() = default;
	SSDX12ConstantBuffer(ID3D12Device* device, const UINT bufferSize = 256);

	UINT GetBufferSize() const { return mCBufferSize; }

protected:
	bool Create(ID3D12Device* device, const UINT bufferSize = 256);
	
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
	if (mBuffer)
	{
		mBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData));

		memcpy(mMappedData, &data, sizeof(T));

		mBuffer->Unmap(0, nullptr);
	}
}


