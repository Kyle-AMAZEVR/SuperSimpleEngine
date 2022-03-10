
#pragma once
#include "SSEngine.h"
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
	virtual void EngineStart() override;
    
	static bool IsInitialized() { return bInitialized; }
	/**
	 * \brief 
	 */
	void ToggleGBufferDumpMode();	

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetImmediateDeviceContext() const;	
	inline IDXGISwapChain* GetSwapChain() const;
	
	void Run() override;

protected:

	static SSDX11Engine* mInstance;

	

private:

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

    static bool bInitialized;

	bool bGbufferDump = false;	
};