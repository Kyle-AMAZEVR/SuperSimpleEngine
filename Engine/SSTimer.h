#pragma once


class ENGINE_API SSGameTimer
{
public:
	SSGameTimer();


	float GetDeltaTime() const;

	void Tick();

protected:
	double SecondsPerCount;
	double DeltaTime;

	__int64 PrevTime;
	__int64 CurrTime;

};