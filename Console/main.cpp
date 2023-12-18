
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

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
		void* PtrMemory = SSMemoryManager::Get().Alloc(size);
		return PtrMemory;
	}

	void operator delete(void* InAddress)
	{

	}

	unsigned int mMember = 0;
};

int main()
{
	SSBitSet Test{};

	Test.Set(0);

	std::cout << Test << std::endl;

	unsigned char result = Test.GetFirstUnsetBit();
	
	std::cout<< result <<std::endl;

	FourByteClass* ptrFourBytes = new FourByteClass{ 10 };

	std::cout << ptrFourBytes->mMember << std::endl;

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