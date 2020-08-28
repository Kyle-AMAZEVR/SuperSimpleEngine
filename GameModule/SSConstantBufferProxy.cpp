#include "SSCommon.h"
#include "SSConstantBufferProxy.h"

SSConstantBufferProxy::~SSConstantBufferProxy()
{
	FreeBufferData();
}

void SSConstantBufferProxy::FreeBufferData()
{
	if (mpBufferData != nullptr && mBufferSize > 0)
	{
		delete mpBufferData;
		mpBufferData = nullptr;
		mBufferSize = 0;
	}
}

SSConstantBufferProxy::SSConstantBufferProxy(const SSConstantBufferProxy& rhs)
{
	if(mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}
	
	mBufferSize = rhs.mBufferSize;
	mpBufferData = new BYTE[mBufferSize];
	
	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);
}

SSConstantBufferProxy::SSConstantBufferProxy(SSConstantBufferProxy&& rhs)
{
	if(mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}
	
	mBufferSize = rhs.mBufferSize;
	mpBufferData = new BYTE[mBufferSize];
	
	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();	
}

SSConstantBufferProxy& SSConstantBufferProxy::operator=(SSConstantBufferProxy&& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	mpBufferData = new BYTE[mBufferSize];

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();
	
	return *this;
}



