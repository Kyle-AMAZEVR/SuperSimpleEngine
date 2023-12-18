
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include "Test.h"
#include "SSTimer.h"

using namespace std::chrono_literals;


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
		//std::cout << "FourByteClass() Constructor"<<mMember << std::endl;
	}
	~FourByteClass()
	{
		//std::cout << "~FourByteClass() Destructor"<<mMember << std::endl;
	}

	
	unsigned int mMember = 0;
};



int main()
{
	SSMemoryAllocator4 TestAllocator{};
	SSGameTimer TestTimer;
	{
		FourByteClass* PtrList[8192] = { nullptr };

		int CurrentIndex = 0;

		TestTimer.Tick();

		for (int i = 0; i < 10000000; ++i)
		{
			if (std::rand() % 2 == 0)
			{
				void* Address = TestAllocator.GetFreeMemory();
				PtrList[CurrentIndex] = new (Address) FourByteClass(std::rand() % 1024);
				CurrentIndex++;
			}
			else if (CurrentIndex > 0)
			{
				PtrList[--CurrentIndex]->~FourByteClass();
				TestAllocator.FreeMemory(PtrList[CurrentIndex]);
			}
		}
		TestTimer.Tick();

		std::cout << "Took " << TestTimer.GetDeltaTime() << std::endl;
	}
	
	{
		FourByteClass* PtrList[8192] = { nullptr };

		int CurrentIndex = 0;

		TestTimer.Tick();

		for (int i = 0; i < 10000000; ++i)
		{
			if (std::rand() % 2 == 0)
			{				
				PtrList[CurrentIndex] = new FourByteClass(std::rand() % 1024);
				CurrentIndex++;
			}
			else if (CurrentIndex > 0)
			{
				delete PtrList[--CurrentIndex];
			}
		}

		TestTimer.Tick();

		std::cout << "Took " << TestTimer.GetDeltaTime() << std::endl;
	}

	char a;
	std::cin >> a;

	return 0;
}