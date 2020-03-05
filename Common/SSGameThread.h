#pragma once


class COMMON_API SSGameThread
{
public:
	void Start(const DWORD GameThreadId);

	static bool IsInGameThread();

	void Tick(float deltaSeconds);

protected:

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	static DWORD mGameThreadId;
};