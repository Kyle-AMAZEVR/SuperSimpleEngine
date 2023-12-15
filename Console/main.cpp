
#include "SSMemoryAllocator.h"
#include <iostream>

int main()
{
	SSBitSet Test{};

	Test.Set(0);

	std::cout << Test << std::endl;

	unsigned char result = Test.GetFirstUnsetBit();
	
	std::cout<< result <<std::endl;

	return 0;
}