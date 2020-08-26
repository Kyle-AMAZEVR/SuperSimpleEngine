#pragma once


class GAMEMODULE_API SSGameTimer
{
public:
	SSGameTimer();


	float GetDeltaTime() const;

	void Tick();

protected:
	double SecondsPerCount;
	double DeltaTime;
	double PCFreq;

	__int64 PrevTime;
	__int64 CurrTime;

};