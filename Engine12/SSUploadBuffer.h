#pragma once

template<class T>

class SSUploadBuffer
{
public:
	SSUploadBuffer(ID3D12Device* device, UINT ElementCount, bool bIsConstantBuffer)
		: mIsConstantBuffer(bIsConstantBuffer)
	{
		mElementByteSize = sizeof(T);
		
		check(ElementCount >= 1);

		if (bIsConstantBuffer)
		{
			mElementByteSize = (sizeof(T) + 255) & (~255);
		}
		

		HR(device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(mElementByteSize * ElementCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&mUploadBuffer)));		

		HR(mUploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&mMappedData)));		
	}

	SSUploadBuffer(const SSUploadBuffer& rhs) = delete;
	SSUploadBuffer& operator=(const SSUploadBuffer& rhs) = delete;


	void CopyData(const UINT ElementIndex, const T& Data)
	{
		memcpy(&mMappedData[ElementIndex*mElementByteSize], &Data, sizeof(T));
	}

	ID3D12Resource* GetResource() const
	{
		return mUploadBuffer.Get();
	}

	~SSUploadBuffer()
	{
		if(mUploadBuffer != nullptr)
		{
			mUploadBuffer->Unmap(0, nullptr);

			mMappedData = nullptr;
		}
	}

protected:
	ComPtr<ID3D12Resource> mUploadBuffer;
	BYTE* mMappedData = nullptr;
	UINT mElementByteSize = 0;
	bool mIsConstantBuffer = false;
};
