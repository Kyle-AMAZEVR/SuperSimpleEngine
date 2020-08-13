
#pragma once

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
	inline ID3D11DeviceContext* GetDeferredDeviceContext() const { return mDeferredContext.Get(); }
	inline IDXGISwapChain* GetSwapChain() const;

	void Run() override;
	void TickGameThread(float deltaTime) override;
	void TickRenderThread(float deltaTime) override;    

protected:

	static SSDX11Engine* mInstance;

	class SSDX11Renderer* mRenderer = nullptr;

private:

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

    static bool bInitialized;

	bool bGbufferDump = false;

private:
	ComPtr<ID3D11Device> mDevice = nullptr;
	ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
	ComPtr<ID3D11DeviceContext> mDeferredContext = nullptr;
	ComPtr<IDXGISwapChain> mSwapChain = nullptr;

    ID3D11Debug* mDebug = nullptr;
	ID3D11SamplerState* mDefaultSamplerState = nullptr;

	

};