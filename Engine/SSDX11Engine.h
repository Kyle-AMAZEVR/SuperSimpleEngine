
#pragma once
#include "SSEngine.h"
#include "Singleton.h"
#include "SSViewport.h"
#include <vector>
#include "SSShader.h"
#include "SSEngineBase.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

// Super Simple
class ENGINE_API SSDX11Engine : public SSEngineBase
{
public:
	static SSDX11Engine& Get();
	static SSDX11Engine* GetPtr();

	SSDX11Engine() = default;	
    virtual void Initialize(HWND windowHandle) override;
	virtual void Shutdown() override;
    virtual void OnWindowResize(int newWidth, int newHeight) override;
    
	static bool IsInitialized() { return bInitialized; }
	/**
	 * \brief 
	 */
	void ToggleGBufferDumpMode();	

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetImmediateDeviceContext() const;	
	inline IDXGISwapChain* GetSwapChain() const;
	
	void Run() override;
	void TickRenderThread(float deltaTime) override;

protected:

	static SSDX11Engine* mInstance;

	class SSDX11Renderer* mRenderer = nullptr;

	class SSGameScene* mCurrentScene = nullptr;

private:

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

    static bool bInitialized;

	bool bGbufferDump = false;	
};