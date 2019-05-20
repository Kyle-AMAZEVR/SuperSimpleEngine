
#include "Core.h"
#include "DXRenderingThread.h"
#include "DXEngine.h"

void DXRenderingThread::Start(HWND handle)
{
    mWindowHandle = handle;

    std::thread t{&DXRenderingThread::Run, this};
    
    mThreadInstance = std::move(t);
}

void DXRenderingThread::Run()
{
    // init engine
    DXEngine::Get().Initialize(mWindowHandle);

    while(1)
    {
        DXEngine::Get().DrawScene();

        if(bRequestExit)
        {
            break;
        }
    }
}

void DXRenderingThread::Join()
{
    mThreadInstance.join();
}