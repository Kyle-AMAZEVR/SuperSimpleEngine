
#include "SSCommon.h"
#include "SSEngineBase.h"
#include "SSGameWindow.h"


void SSEngineBase::EngineStart()
{
	// start game thread
	mGameThread = new SSGameThread();
	mGameThread->Start(GetCurrentThreadId());

	// start rendering thread
	mRenderingThread = new SSRenderingThread();
	mRenderingThread->Start(SSGameWindow::GetPtr()->GetWindowHandle(), this);
}


SSGameThread* SSEngineBase::mGameThread = nullptr;
SSRenderingThread* SSEngineBase::mRenderingThread = nullptr;
DWORD SSEngineBase::MainThreadId = 0;