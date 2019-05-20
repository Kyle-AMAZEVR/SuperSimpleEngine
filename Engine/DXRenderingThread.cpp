
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
    mRenderingThreadId = std::this_thread::get_id();

    // init engine
    DXEngine::Get().Initialize(mWindowHandle);

    while(1)
    {   
        // consume command queue
        {
            std::lock_guard guard(mCommandQueueMutex);
            if(mCommandQueue.empty() == false)
            {
                for(auto iter = mCommandQueue.begin(); iter != mCommandQueue.end(); iter++)
                {
                    (*iter)();
                }
                mCommandQueue.clear();
            }
        }

        DXEngine::Get().DrawScene();

        if(bRequestExit)
        {
            return;
        }
    }
}

void DXRenderingThread::Join()
{
    mThreadInstance.join();
}

bool DXRenderingThread::IsInRenderingThread()
{
    if(mRenderingThreadId == std::this_thread::get_id())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void DXRenderingThread::ExecuteInRenderingThread(std::function<void()>&& lambdaFunction)
{
    if(IsInRenderingThread())
    {
        lambdaFunction();
    }
    else
    {
        std::lock_guard guard(mCommandQueueMutex);
        mCommandQueue.push_back(std::move(lambdaFunction));
    }    
}

std::thread::id DXRenderingThread::mRenderingThreadId;