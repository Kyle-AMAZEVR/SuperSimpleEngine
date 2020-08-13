
#include "Core.h"
#include "SSEngine.h"
#include "SSTexture2D.h"
#include "SSObjMesh.h"
#include "SSDX11Renderer.h"

bool SSDX11Engine::bInitialized = false;

SSDX11Engine* SSDX11Engine::mInstance = nullptr;

SSDX11Engine& SSDX11Engine::Get()
{
	if(mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return (*mInstance);
}

SSDX11Engine* SSDX11Engine::GetPtr()
{
	if (mInstance == nullptr)
	{
		mInstance = new SSDX11Engine();
	}

	return mInstance;
}

ID3D11Device* SSDX11Engine::GetDevice() const
{
	return mRenderer->GetDevice();
}

ID3D11DeviceContext* SSDX11Engine::GetImmediateDeviceContext() const
{
	return mRenderer->GetImmediateDeviceContext();
}

inline IDXGISwapChain* SSDX11Engine::GetSwapChain() const
{
	return mRenderer->GetSwapChain();
}

void SSDX11Engine::Initialize(HWND windowHandle)
{
	mRenderer = new SSDX11Renderer();
	mRenderer->Initialize(windowHandle);

	mRenderingThread->SetRenderer(mRenderer);
	
	mGameThread = new SSGameThread(GetCurrentThreadId());
	
    mWindowHandle = windowHandle;

	bInitialized = true;
}

void SSDX11Engine::ToggleGBufferDumpMode()
{
	bGbufferDump = !bGbufferDump;
}

void SSDX11Engine::Shutdown()
{
	if (mRenderingThread)
	{
		// wait for rendering thread		
		mRenderingThread->RequestExit();
		mRenderingThread->Join();

		delete mRenderingThread;
		mRenderingThread = nullptr;
	}
	
	if(mRenderer)
	{
		mRenderer = nullptr;
	}

	if(mGameThread)
	{
		delete mGameThread;
		mGameThread = nullptr;
	}

	
}


void SSDX11Engine::OnWindowResize(int newWidth, int newHeight)
{
	if (bInitialized)
	{
		if(mRenderer)
		{
			mRenderer->OnWindowResize(newWidth, newHeight);
		}
	}
}


void SSDX11Engine::Run()
{
	EngineStart();

	while(!bRequestExit)
	{
		mGameThread->Tick(0.033f);
	}

	Shutdown();
}

void SSDX11Engine::TickGameThread(float deltaTime)
{
	
}
void SSDX11Engine::TickRenderThread(float deltaTime)
{
	if (mRenderer)
	{
		mGameThread->WaitForGameThread(10);
		mRenderer->DrawScene();
	}
}

