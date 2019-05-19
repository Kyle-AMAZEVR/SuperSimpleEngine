
#pragma once

#include "Singleton.h"

class ENGINE_API DXEngine : public Singleton<DXEngine>
{
public:    
    bool Initialize(HWND windowHandle);

private:
    bool CreateDevice();
    bool CreateSwapChain();

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

private:
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext;
    IDXGISwapChain* mSwapChain = nullptr;
};