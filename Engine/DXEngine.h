
#pragma once

#include "Singleton.h"
#include "DXViewport.h"
#include "DXVertexBuffer.h"
#include "DXIndexBuffer.h"
#include "DXShader.h"

class ENGINE_API DXEngine : public Singleton<DXEngine>
{
public:    
    bool Initialize(HWND windowHandle);
    void UnInitialize();
    void OnWindowResize(int newWidth, int newHeight);
    static bool IsInitialized() { return bInitialized; }

    inline ID3D11Device* GetDevice() { return mDevice; }
    inline ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
    inline IDXGISwapChain* GetSwapChain() {return mSwapChain;}    

    void DrawScene();

protected:
    int mBufferWidth = 1024;
    int mBufferHeight = 768;

    DXViewport mViewport;

    void TestCompileShader();
    void TestCreateResources();

private:
    bool CreateDevice();
    bool CreateSwapChain();

    HWND mWindowHandle;
    UINT m4xMSAAQuality;

    static bool bInitialized;

private:
    ID3D11Device* mDevice = nullptr;
    ID3D11DeviceContext* mDeviceContext = nullptr;
    IDXGISwapChain* mSwapChain = nullptr;
    ID3D11Debug* mDebug = nullptr;

    // 
    std::shared_ptr<DXVertexBuffer> mTestVertexBuffer;
    std::shared_ptr<DXIndexBuffer> mTestIndexBuffer;
    std::shared_ptr<DXVertexShader> mTestVertexShader;
    //
};