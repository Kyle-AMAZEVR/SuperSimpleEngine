
#pragma once

#include "Singleton.h"
#include "DXViewport.h"

#include "SSShader.h"

// Super Simple
class ENGINE_API SSEngine : public Singleton<SSEngine>
{
public:
	SSEngine() = default;

    bool Initialize(HWND windowHandle);
	void Shutdown();
    void OnWindowResize(int newWidth, int newHeight);
    static bool IsInitialized() { return bInitialized; }

	void SetVertexShader(std::weak_ptr<SSVertexShader> vs);
	void SetPixelShader(std::weak_ptr<SSPixelShader> ps);

    inline ID3D11Device* GetDevice() { return mDevice; }
    inline ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
    inline IDXGISwapChain* GetSwapChain() {return mSwapChain;}    

    void DrawScene();

protected:
    int mBufferWidth = 1024;
    int mBufferHeight = 768;

    SSViewport mViewport;

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
	ID3D11SamplerState* mDefaultSamplerState = nullptr;

	std::shared_ptr<class SSCube> mTestCube;

    // 
    std::shared_ptr<class SSVertexBuffer> mTestVertexBuffer;
    std::shared_ptr<class SSPixelShader> mTestPixelShader;
    std::shared_ptr<class SSIndexBuffer> mTestIndexBuffer;
    std::shared_ptr<class SSVertexShader> mTestVertexShader;
	std::shared_ptr<class SSTexture2D> mTestTexture;

    //
};