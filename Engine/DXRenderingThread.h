#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <deque>

class ENGINE_API DXRenderingThread 
{
public:
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

    void ExecuteInRenderingThread(std::function<void()>&& lambdaFunction);

    static inline bool IsInRenderingThread();

protected:
    void Run();   

    //
    std::mutex mCommandQueueMutex; 
    std::deque<std::function<void()>> mCommandQueue; 
    //

    //
    std::thread mThreadInstance;
    static std::thread::id mRenderingThreadId;
    //

    HWND mWindowHandle;
    bool bRequestExit = false;
};