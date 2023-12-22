
#include "SSMemoryAllocator.h"
#include <bitset>

bool SSBitSet::IsSet(const unsigned char InIndex) const
{
	unsigned int Shifted = mBit >> InIndex;
	return Shifted & 0b00000000'00000000'00000000'00000001;
}

inline void SSBitSet::Set(const unsigned char InIndex)
{
	unsigned int Temp = 0b00000000'00000000'00000000'00000001 << InIndex;
	mBit = mBit | Temp;
}

inline void SSBitSet::UnSet(const unsigned char InIndex)
{
	unsigned int Temp = 0b00000000'00000000'00000000'00000001 << InIndex;
	mBit = mBit ^ Temp;
}

inline bool SSBitSet::IsAllSet() const
{
	unsigned int MaxValue = 0;
	MaxValue -= 1;
	return mBit == MaxValue;
}

inline bool SSBitSet::IsAnySet() const
{
	return mBit > 0;
}

inline void SSBitSet::SetFirstUnSetBit()
{
	Set(GetZeroesOnRight(~mBit));
}

std::ostream& operator<<(std::ostream& os, const SSBitSet& InBitSet)
{
	os << std::bitset<32>(InBitSet.mBit);
	return os;
}
inline unsigned char SSBitSet::GetZeroesOnRight(unsigned int InBitValue)
{
	unsigned int c = InBitValue;  // your input number
	int r;           // result goes here
	static const int MultiplyDeBruijnBitPosition[32] =
	{
	  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
	  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};
	r = MultiplyDeBruijnBitPosition[((uint32_t)((c & -c) * 0x077CB531U)) >> 27];
	return r;
}

unsigned char SSBitSet::GetZeroesOnRight() const
{
	// https://graphics.stanford.edu/~seander/bithacks.html
	unsigned int c = mBit;  // your input number
	int r;           // result goes here
	static const int MultiplyDeBruijnBitPosition[32] =
	{
	  0, 1, 28, 2, 29, 14, 24, 3, 30, 22, 20, 15, 25, 17, 4, 8,
	  31, 27, 13, 23, 21, 19, 16, 7, 26, 12, 18, 6, 11, 5, 10, 9
	};
	r = MultiplyDeBruijnBitPosition[((uint32_t)((c & -c) * 0x077CB531U)) >> 27];
	return r;
}

unsigned char SSBitSet::GetFirstUnsetBit() const
{
	return GetZeroesOnRight(~mBit);
}


void SSMemoryManager::DeAlloc(void* InAddress, int InSizeHint)
{
	if (InSizeHint == 4)
	{
		mFourBytesAllocator.FreeMemory(InAddress);
	}
	else
	{

	}
}

void* SSMemoryManager::Alloc(size_t InSize)
{
	if (InSize == 4)
	{
		return mFourBytesAllocator.GetFreeMemory();
	}
	else if (InSize == 8)
	{
		return mEightBytesAllocator.GetFreeMemory();
	}
	else if (InSize == 16)
	{
		return mSixteenBytesAllocator.GetFreeMemory();
	}
	else if (InSize == 24)
	{
		return mTwentyFourBytesAllocator.GetFreeMemory();
	}
	else if (InSize == 32)
	{
		return mThirtyTwoBytesAllocator.GetFreeMemory();
	}

	check(false);

	return nullptr;
}

void* SSAlignedMemoryAllocator::GetFreeMemory(size_t size)
{
	check(size == 16|| size ==32 || size==64 || size==128 || size==256)

	return nullptr;
}