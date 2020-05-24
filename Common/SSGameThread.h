#pragma once

// game thread is the same as main thread
class COMMON_API SSGameThread
{
public:

	void CreateGameWindow();

	void Start(DWORD gameThreadId);

	static bool IsInGameThread();

	void Tick(float deltaSeconds);

	HANDLE GetGameThreadEventHandle() { return mGameThreadDoneEventHandle; }

	void WaitForGameThread(const DWORD WaitTime = INFINITE);

protected:

	DWORD Run();

	static DWORD Run(LPVOID param);

	static DWORD mGameThreadId;

	HANDLE mThreadHandle = nullptr;

	HANDLE mGameThreadDoneEventHandle = nullptr;

	
};