#pragma once


class COMMON_API SSGameThread
{
public:
	void Start(const DWORD GameThreadId);

	static bool IsInGameThread();

	void Tick(float deltaSeconds);

	HANDLE GetGameThreadEventHandle() { return mGameThreadDoneEventHandle; }

protected:

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	static DWORD mGameThreadId;
};