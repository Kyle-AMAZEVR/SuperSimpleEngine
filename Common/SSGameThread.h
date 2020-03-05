#pragma once


class COMMON_API SSGameThread
{
public:
	void SetGameThreadId(const DWORD GameThreadId) { mGameThreadId = GameThreadId; }

	static bool IsInGameThread();

protected:

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	static DWORD mGameThreadId;
};