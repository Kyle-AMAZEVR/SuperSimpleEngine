
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

int main()
{
	SSBitSet Test{};

	Test.Set(0);

	std::cout << Test << std::endl;

	unsigned char result = Test.GetFirstUnsetBit();
	
	std::cout<< result <<std::endl;

	SSMemoryAllocator4 TestAllocator{};

	void* PtrList[1024] = { nullptr };

	int CurrentIndex = 0;

	while (1)
	{
		std::this_thread::sleep_for(100ms);

		if (std::rand() % 2 == 0)
		{
			PtrList[CurrentIndex++] = TestAllocator.GetFreeMemory();
			std::cout << "Allocated : \t"<< CurrentIndex << std::endl;
		}
		else if (CurrentIndex > 0)
		{			
			TestAllocator.FreeMemory(PtrList[--CurrentIndex]);
			std::cout << "DeAllocated : \t" << CurrentIndex << std::endl;
		}
	}

	unsigned int* Hello = (unsigned int*)TestAllocator.GetFreeMemory();

	*Hello = 4;

	std::cout << *Hello << std::endl;	

	TestAllocator.FreeMemory(Hello);

	return 0;
}