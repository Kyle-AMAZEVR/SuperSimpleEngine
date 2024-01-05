
#include "SSMemoryAllocator.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <deque>
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
	unsigned int mMember2 = 0;
};


class FVirtualClass
{
public:
	virtual void Printinfo();
};

class FEmptyClass
{

};



int main()
{
	SSMemoryAllocator8 TestAllocator{};
	SSGameTimer TestTimer;
	{
		FourByteClass* PtrList[8192] = { nullptr };

		std::deque<FourByteClass*> PtrDeque;

		int CurrentIndex = 0;

		TestTimer.Tick();

		for (int i = 0; i < 10000000; ++i)
		{
			if (std::rand() % 2 == 0)
			{
				void* Address = TestAllocator.GetFreeMemory();
				PtrDeque.push_back(new (Address) FourByteClass(std::rand() % 1024));				
			}
			else if (PtrDeque.size() > 0)
			{
				FourByteClass* Ptr = PtrDeque.front();
				Ptr->~FourByteClass();
				TestAllocator.FreeMemory(Ptr);
				PtrDeque.pop_front();
			}
		}
		TestTimer.Tick();

		std::cout << "Took " << TestTimer.GetDeltaTime() << std::endl;
	}
	
	{
		std::deque<FourByteClass*> PtrDeque;


		TestTimer.Tick();

		for (int i = 0; i < 10000000; ++i)
		{
			if (std::rand() % 2 == 0)
			{				
				PtrDeque.push_back(new FourByteClass(std::rand() % 1024));
				//CurrentIndex++;
			}
			else if (PtrDeque.size() > 0)
			{
				FourByteClass* Ptr = PtrDeque.front();
				delete Ptr;
				PtrDeque.pop_front();
			}
		}

		TestTimer.Tick();

		std::cout << "Took " << TestTimer.GetDeltaTime() << std::endl;
	}

	char a;
	std::cin >> a;

	return 0;
}