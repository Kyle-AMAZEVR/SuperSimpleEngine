
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Test.h"

using namespace std::chrono_literals;

void* operator new(size_t size)
{
	void* PtrMemory = SSMemoryManager::Get().Alloc(size);
	return PtrMemory;
}

template<class T>
class TSizeofClass
{
public:
	static constexpr int Size = sizeof(T);
};

class FourByteClass
{
public:
	FourByteClass(unsigned int InMember) : mMember(InMember)
	{
		std::cout << "FourByteClass() Constructor"<<mMember << std::endl;
	}
	~FourByteClass()
	{
		std::cout << "~FourByteClass() Destructor"<<mMember << std::endl;
	}

	void* operator new(size_t size)
	{
		void* PtrMemory = SSMemoryManager::Get().Alloc(TSizeofClass<FourByteClass>::Size);
		return PtrMemory;
	}

	void operator delete(void* InAddress)
	{
		SSMemoryManager::Get().DeAlloc(InAddress, TSizeofClass<FourByteClass>::Size);
	}

	unsigned int mMember = 0;
};



int main()
{
	
	SSBitSet Test{};

	auto UnsetBitPos = Test.GetFirstUnsetBit();

	std::cout << UnsetBitPos << std::endl;

	SSBitSet Test2{ 0b1111'0000'1111'0000'1111'0000'1111'0000 };

	bool HasUnset = Test2.HasUnset(5);

	std::cout << HasUnset << std::endl;

	/*SSMemoryAllocator4 TestAllocator{};

	FourByteClass* PtrList[1024] = { nullptr };

	int CurrentIndex = 0;

	while (1)
	{
		std::this_thread::sleep_for(200ms);

		if (std::rand() % 2 == 0)
		{
			void* Address = TestAllocator.GetFreeMemory();
			PtrList[CurrentIndex] = new (Address) FourByteClass(std::rand() % 1024);
			std::cout << "Allocated : \t"<< CurrentIndex++ << std::endl;
		}
		else if (CurrentIndex > 0)
		{
			PtrList[--CurrentIndex]->~FourByteClass();
			TestAllocator.FreeMemory(PtrList[CurrentIndex]);
			std::cout << "DeAllocated : \t" << CurrentIndex << std::endl;
		}
	}

	unsigned int* Hello = (unsigned int*)TestAllocator.GetFreeMemory();

	*Hello = 4;

	std::cout << *Hello << std::endl;	

	TestAllocator.FreeMemory(Hello);
	*/

	return 0;
}