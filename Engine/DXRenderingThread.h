#pragma once

#include <thread>

class ENGINE_API DXRenderingThread 
{
public:
    void Start(HWND handle);
    void Join();
    void RequestExit() { bRequestExit = true;}

    

protected:
    void Run();

    std::thread mThreadInstance;
    HWND mWindowHandle;
    bool bRequestExit = false;
};