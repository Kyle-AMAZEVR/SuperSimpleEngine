
#include "SSEngineBase.h"
#include "SSGameWindow.h"
#include "SSGameThread.h"



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