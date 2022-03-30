#include "SSCore.h"
#include "SSConatantBufferData.h"

SSConatantBufferData::~SSConatantBufferData()
{
	FreeBufferData();
}

void SSConatantBufferData::FreeBufferData()
{
	if (mpBufferData != nullptr && mBufferSize > 0)
	{
		delete mpBufferData;
		mpBufferData = nullptr;
		mBufferSize = 0;
	}
}

SSConatantBufferData::SSConatantBufferData(const SSConatantBufferData& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);
}

SSConatantBufferData::SSConatantBufferData(SSConatantBufferData&& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();
}

SSConatantBufferData& SSConatantBufferData::operator = (SSConatantBufferData&& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	rhs.FreeBufferData();

	return *this;
}

SSConatantBufferData& SSConatantBufferData::operator=(const SSConatantBufferData& rhs)
{
	if (mBufferSize != rhs.mBufferSize)
	{
		FreeBufferData();
	}

	mBufferSize = rhs.mBufferSize;
	
	if (mpBufferData == nullptr)
	{
		mpBufferData = new BYTE[mBufferSize];
	}

	memcpy_s(mpBufferData, mBufferSize, rhs.mpBufferData, mBufferSize);

	return *this;
}



