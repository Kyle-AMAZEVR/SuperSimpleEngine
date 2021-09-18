
#include "Core.h"
#include "SSDX11Engine.h"
#include "SSTexture2D.h"
#include "SSObjMesh.h"
#include "SSDX11Renderer.h"
#include "SSDX11CubeScene.h"
#include "SSGameObjectManager.h"
#include "SSRenderingObjectManager.h"
#include "SSSharedRenderData.h"

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
	
    mWindowHandle = windowHandle;	

	mCurrentScene = new SSDX11CubeScene();
	mCurrentScene->InitializeScene();
	
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
	SSSharedRenderData::Get().Initialize();
	
	EngineStart();

	// Main message loop
	MSG msg = { 0 };
	while (!bRequestExit)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			mGameThread->Tick();
		}
	}

	Shutdown();
}

void SSDX11Engine::TickRenderThread(float deltaTime)
{
	if (mRenderer)
	{
		SSRenderingObjectManager::Get().Tick(deltaTime);

		mRenderer->DrawScene();
	}
}

