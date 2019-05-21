#pragma once

#include <thread>
#include <functional>
#include <mutex>
#include <deque>
#include "Singleton.h"

class ENGINE_API DXRenderingThread : public Singleton<DXRenderingThread>
{
public:
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

    void ExecuteInRenderingThread(std::function<void()>&& lambdaFunction);

    static bool IsInRenderingThread();

protected:
    void Run();

    //@ commandqueue mutex
    std::mutex mCommandQueueMutex; 
    std::deque<std::function<void()>> mCommandQueue; 
    //

    //@ thread related 
    std::thread mThreadInstance;
    static std::thread::id mRenderingThreadId;
    //

    HWND mWindowHandle;

    //@ exit thread when bRequestExit == true
    bool bRequestExit = false;
};