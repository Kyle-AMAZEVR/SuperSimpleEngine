#pragma once

#include <thread>

class ENGINE_API DXRenderingThread 
{
public:
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

    void ExecuteInRenderingThread();

    static bool IsInRenderingThread();

protected:
    void Run();

    std::thread mThreadInstance;
    static std::thread::id mRenderingThreadId;
    HWND mWindowHandle;
    bool bRequestExit = false;
};