#pragma once

// game thread is the same as main thread
class COMMON_API SSGameThread
{
public:	
	SSGameThread(DWORD gameThreadId);

	static bool IsInGameThread();

	void Tick(float deltaSeconds);

	HANDLE GetGameThreadEventHandle() { return mGameThreadDoneEventHandle; }	

	void WaitForGameThread(const DWORD WaitTime = INFINITE);

protected:
	void Start(DWORD gameThreadId);

	DWORD Run();

	static DWORD Run(LPVOID param);

	static DWORD mGameThreadId;

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	bool bRequestExist = false;
};