#pragma once


class GAMEMODULE_API SSConstantBufferProxy
{
public:
	SSConstantBufferProxy(){}
	
	template<class T>
	SSConstantBufferProxy(const T& value);

	SSConstantBufferProxy(const SSConstantBufferProxy& rhs);

	SSConstantBufferProxy(SSConstantBufferProxy&& rhs);

	SSConstantBufferProxy& operator=(SSConstantBufferProxy&& rhs);

	SSConstantBufferProxy& operator=(const SSConstantBufferProxy& rhs);

	~SSConstantBufferProxy();
		
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
void SSConstantBufferProxy::SetBufferData(const T& value)
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
SSConstantBufferProxy::SSConstantBufferProxy(const T& value)
{	
	SetBufferData(value);
}