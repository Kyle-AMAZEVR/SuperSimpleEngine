
#pragma once
#include "SSCore.h"
#include "Singleton.h"
#include <iostream>

class CORE_API SSBitSet
{
public:
	SSBitSet()
		: mBit(0)
	{
	}

	static inline unsigned char GetZeroesOnRight(unsigned int);
	inline bool IsSet(const unsigned char InIndex) const;
	inline void Set(const unsigned char InIndex);
	inline void UnSet(const unsigned char InIndex);
	inline bool IsAllSet() const;
	inline bool IsAnySet() const;
	inline unsigned char GetZeroesOnRight() const;	
	inline unsigned char GetFirstUnsetBit() const;
	inline void SetFirstUnSetBit();	
	
	CORE_API friend std::ostream& operator<<(std::ostream& os, const SSBitSet& InBitSet);

private:
	unsigned int mBit;
};

template<unsigned int TAllocSize>
class SSFixedMemoryAllocator
{
public:
	SSFixedMemoryAllocator();
	~SSFixedMemoryAllocator();	

	void* GetFreeMemory();
	bool  FreeMemory(void* InAddress);
	float GetOccupiedRate();

private:
	UINT64		mStartAddress;
	int			mAllocationSize = TAllocSize;
	int			mTotalCount = 0;
	int			mAllocatedCount = 0;
	void*		mMemoryPool;
	static const int	mBucketSize = 1024;
	SSBitSet	mBitSetBucket[mBucketSize]; // 4byte * 1024 
};

template<unsigned int TAllocSize>
SSFixedMemoryAllocator<TAllocSize>::SSFixedMemoryAllocator()
{
	mMemoryPool		= std::malloc(mBucketSize * 32 * TAllocSize);
	mTotalCount		= mBucketSize * 32;
	mStartAddress	= (UINT64)mMemoryPool;
}

template<unsigned int TAllocSize>
SSFixedMemoryAllocator<TAllocSize>::~SSFixedMemoryAllocator()
{
	std::free(mMemoryPool);
}

template<unsigned int TAllocSize>
void* SSFixedMemoryAllocator<TAllocSize>::GetFreeMemory()
{
	for (int i = 0; i < mBucketSize; ++i)
	{
		if (!mBitSetBucket[i].IsAllSet())
		{
			int j = mBitSetBucket[i].GetFirstUnsetBit();
			
			check(!mBitSetBucket[i].IsSet(j));

			mBitSetBucket[i].Set(j);

			mAllocatedCount++;

			void* ResultAddress = reinterpret_cast<void*>(reinterpret_cast<unsigned char*>(mMemoryPool) + ((32 * TAllocSize) * i + (TAllocSize * j)));

			return ResultAddress;
		}
	}

	// out of memory
	check(false);
	
	return nullptr;
}

template<unsigned int TAllocSize>
bool SSFixedMemoryAllocator<TAllocSize>::FreeMemory(void* InAddress)
{
	UINT64 TempAddress			= (UINT64)InAddress;

	unsigned int BucketIndex	= (TempAddress - mStartAddress) / (32 * TAllocSize);

	unsigned int BitIndex		= ((TempAddress - mStartAddress) % (32 * TAllocSize)) / TAllocSize;

	check(mBitSetBucket[BucketIndex].IsSet(BitIndex));

	mBitSetBucket[BucketIndex].UnSet(BitIndex);

	mAllocatedCount--;

	return true;
}

template<unsigned int TAllocSize>
float SSFixedMemoryAllocator<TAllocSize>::GetOccupiedRate()
{
	return static_cast<float>(mAllocatedCount) / mTotalCount;
}

class CORE_API SSMemoryAllocator4 : public SSFixedMemoryAllocator<4>
{
};

class CORE_API SSMemoryAllocator8 : public SSFixedMemoryAllocator<8>
{
};

class CORE_API SSMemoryAllocator16 : public SSFixedMemoryAllocator<16>
{
};

class CORE_API SSMemoryAllocator32 : public SSFixedMemoryAllocator<32>
{
};

class CORE_API SSMemoryAllocator64 : public SSFixedMemoryAllocator<64>
{
};

class CORE_API SSMemoryAllocator128 : public SSFixedMemoryAllocator<128>
{
};

class CORE_API SSMemoryManager : public Singleton<SSMemoryManager>
{
public:
	void* Alloc(size_t Size);
	void  DeAlloc(void* InAddress, int SizeHint = -1);
protected:
	SSMemoryAllocator4	mFourBytesAllocator;
	SSMemoryAllocator8	mEightBytesAllocator;
	SSMemoryAllocator16 mSixteenBytesAllocator;
	SSMemoryAllocator32 mThirtyTwoBytesAllocator;
};