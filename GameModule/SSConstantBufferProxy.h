#pragma once


class GAMEMODULE_API SSConstantBufferProxy
{
public:
	template<class T>
	SSConstantBufferProxy(const T& value);

	SSConstantBufferProxy(const SSConstantBufferProxy& rhs);

	SSConstantBufferProxy(SSConstantBufferProxy&& rhs);

	~SSConstantBufferProxy();
		
	template<class T>
	void SetBufferData(const T& value);

	BYTE* GetData() const { return mpBufferData; }
	
protected:

	void FreeBufferData();	
	BYTE* mpBufferData = nullptr;
	int mBufferSize = 0;
};


template<class T>
void SSConstantBufferProxy::SetBufferData(const T& value)
{
	if(mpBuffData == null || mBufferSize != sizeof(T))
	{
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