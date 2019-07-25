#include "Core.h"
#include "SSTimer.h"


SSGameTimer::SSGameTimer()
	: SecondsPerCount(0.0)
{
	__int64 CounterPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&CounterPerSec);
	SecondsPerCount = 1.0 / static_cast<double>(CounterPerSec);
}

void SSGameTimer::Tick()
{
	__int64 TempTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&TempTime);
	CurrTime = TempTime;

	DeltaTime = (CurrTime - PrevTime) * SecondsPerCount;

	PrevTime = CurrTime;

	if (DeltaTime < 0)
	{
		DeltaTime = 0.0;
	}
}

float SSGameTimer::GetDeltaTime() const
{
	return static_cast<float>(DeltaTime);
}