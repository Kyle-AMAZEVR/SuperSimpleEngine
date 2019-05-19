
#pragma once

#include "Singleton.h"

class ENGINE_API DXEngine : public Singleton<DXEngine>
{
public:    
    bool Initialize(HWND windowHandle);
    void OnWindowResize(int newWidth, int newHeight);

protected:
    int mBufferWidth = 1024;
    int mBufferHeight = 768;

private:
    bool CreateDevice();
    bool CreateSwapChain();

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

private:
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;    
    ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	D3D11_VIEWPORT mScreenViewport;

};