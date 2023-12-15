
#pragma once
#include "SSCore.h"
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
	
	
	CORE_API friend std::ostream& operator<<(std::ostream& os, const SSBitSet& InBitSet);

private:
	unsigned int mBit;
};

class CORE_API SSFixedMemoryAllocator
{
public:
	SSFixedMemoryAllocator(const int AllocationSize, const int TotalSize);
	void* GetFreeMemory();

private:
	int		mAllocationSize = 0;
	int		mTotalSize = 0;
	int		mCount = 0;
	void*	MemoryPool;

};

