#pragma once


class CORE_API SSConatantBufferData
{
public:
	SSConatantBufferData(){}
	
	template<class T>
	SSConatantBufferData(const T& value);

	SSConatantBufferData(const SSConatantBufferData& rhs);

	SSConatantBufferData(SSConatantBufferData&& rhs);

	SSConatantBufferData& operator=(SSConatantBufferData&& rhs);

	SSConatantBufferData& operator=(const SSConatantBufferData& rhs);

	~SSConatantBufferData();
		
	template<class T>
	void SetBufferData(const T& value);

	BYTE* GetData() const { return mpBufferData; }

	UINT GetBufferSize() const { return mBufferSize; }
	
protected:

	void FreeBufferData();	
	BYTE* mpBufferData = nullptr;
	int mBufferSize = 0;
};


template<class T>
void SSConatantBufferData::SetBufferData(const T& value)
{	
	if(mBufferSize != sizeof(T))
	{
		FreeBufferData();
		mBufferSize = sizeof(T);
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, &value, mBufferSize);
}

template<class T>
SSConatantBufferData::SSConatantBufferData(const T& value)
{	
	SetBufferData(value);
}