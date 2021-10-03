
#include "SSEngineBase.h"
#include "SSGameWindow.h"
#include "SSGameThread.h"


void SSEngineBase::EngineStart()
{
	// start game thread
	mGameThread = new SSGameThread(GetCurrentThreadId());	

	// start rendering thread
	mRenderingThread = new SSRenderingThread();
	mRenderingThread->Start(SSGameWindow::GetPtr()->GetWindowHandle(), this);
}

void SSEngineBase::WaitForGameThread(const DWORD WaitTime)
{
	mGameThread->WaitForGameThread(WaitTime);
}
void SSEngineBase::WaitForRenderingTheread(const DWORD WaitTime)
{

}


SSGameThread* SSEngineBase::mGameThread = nullptr;
SSRenderingThread* SSEngineBase::mRenderingThread = nullptr;
DWORD SSEngineBase::MainThreadId = 0;

SSEngineBase* GEngine = nullptr;