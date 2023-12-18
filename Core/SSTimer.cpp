
#include "SSTimer.h"


SSGameTimer::SSGameTimer()
	: SecondsPerCount(0.0)
{
	LARGE_INTEGER li;
	QueryPerformanceFrequency(&li);
	PCFreq = double(li.QuadPart);

	QueryPerformanceCounter(&li);
	PrevTime = li.QuadPart;
}

void SSGameTimer::Start()
{
	Tick();
}

void SSGameTimer::Stop()
{
	Tick();
}

void SSGameTimer::Tick()
{
	LARGE_INTEGER Now;
	QueryPerformanceCounter(&Now);	

	DeltaTime = double(Now.QuadPart - PrevTime) / PCFreq;
	PrevTime = Now.QuadPart;
}

float SSGameTimer::GetDeltaTime() const
{
	return static_cast<float>(DeltaTime);
}