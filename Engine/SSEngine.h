
#pragma once

#include "Singleton.h"
#include "SSViewport.h"

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

	void ChangeToNextDumpMode();

    inline ID3D11Device* GetDevice() { return mDevice; }
    inline ID3D11DeviceContext* GetDeviceContext() { return mDeviceContext; }
    inline IDXGISwapChain* GetSwapChain() {return mSwapChain;}    

    void DrawScene();

protected:
    int mBufferWidth = 1024;
    int mBufferHeight = 768;

    std::shared_ptr<SSViewport> mViewport = nullptr;

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

	std::shared_ptr<class SSMaterial> mTestMaterial;
	std::shared_ptr<class SSMaterial> mTBNDebugMaterial;
	std::shared_ptr<class SSMaterial> mDeferredLightMaterial;

	std::shared_ptr<class SSCube> mTestCube;
	std::shared_ptr<class SSSphere> mTestSphere;
	std::shared_ptr<class SSScreenBlit> mScreenBlit;
    // 
    std::shared_ptr<class SSVertexBuffer> mTestVertexBuffer;
	std::shared_ptr<class SSIndexBuffer> mTestIndexBuffer;

	std::shared_ptr<class SSVertexShader> mTestVertexShader;
    std::shared_ptr<class SSPixelShader> mTestPixelShader;

	std::shared_ptr<class SSVertexShader> mDeferredVertexShader;
	std::shared_ptr<class SSPixelShader> mDeferredPixelShader;

	std::shared_ptr<class SSVertexShader> mDeferredLightVertexShader;
	std::shared_ptr<class SSPixelShader> mDeferredLightPixelShader;

	std::shared_ptr<class SSTextureCube> mTestCubeTexture;

	std::shared_ptr<class SSVertexShader> mCubemapVertexShader;
	std::shared_ptr<class SSPixelShader> mCubemapPixelShader;

	std::shared_ptr<class SSVertexShader> mEquirectToCubemapVertexShader;
	std::shared_ptr<class SSPixelShader> mEquirectToCubemapPixelShader;

	std::shared_ptr<class SSVertexShader> mCubemapConvolutionVertexShader;
	std::shared_ptr<class SSPixelShader> mCubemapConvolutionPixelShader;

	std::shared_ptr<class SSVertexShader> mPrefilterVertexShader;
	std::shared_ptr<class SSPixelShader> mPrefilterPixelShader;

	std::shared_ptr<class SSVertexShader> m2DLUTVertexShader;
	std::shared_ptr<class SSPixelShader> m2DLUTPixelShader;

	std::shared_ptr<class SSVertexShader> mTBNDebugVertexShader;
	std::shared_ptr<class SSPixelShader> mTBNDebugPixelShader;
	

	// @ cubemap rendertargets
	std::shared_ptr<class SSCubemapRenderTarget> mEquirectToCubemapRenderTarget;
	std::shared_ptr<class SSCubemapRenderTarget> mConvolutionRenderTarget;
	std::shared_ptr<class SSPrefilterCubemapRenderTarget> mPrefilterRenderTarget;

	std::shared_ptr<class SSTextureCube> mEnvCubemap;
	std::shared_ptr<class SSTextureCube> mEnvCubemapConvolution;
	std::shared_ptr<class SSTextureCube> mEnvCubemapPrefilter;
	std::shared_ptr<class SSTexture2D> m2DLUTTexture;
	// @
	
    
	std::shared_ptr<class SSTexture2D> mNormalTexture;
	std::shared_ptr<class SSTexture2D> mRoughnessTexture;
	std::shared_ptr<class SSTexture2D> mMetalicTexture;
	std::shared_ptr<class SSTexture2D> mDiffuseTexture;
	std::shared_ptr<class SSTexture2D> mHDREnvmap;


	std::shared_ptr<class SSGBuffer> mGBuffer;
	
	std::shared_ptr<class SSGenericRenderTarget> m2DLUTRenderTarget;
    //

	std::shared_ptr<class SSObjMesh> mSponzaMesh;
	std::shared_ptr<class SSObjMesh> mObjMeshSphere;

	std::shared_ptr<class SSPostProcess> mFXAAPostProcess;
	std::shared_ptr<class SSGBufferDumpPostProcess> mGBufferDumpProcess;

private:
	bool TryLoadEnvCubemap(std::wstring filepath);
	bool TryLoadEnvCubemapConvolution(std::wstring filepath);
	bool TryLoadEnvCubemapPrefilter(std::wstring filepath);
	bool TryLoad2DLUTTexture();

	void CreateEnvCubemap();
	void CreateEnvCubemapConvolution();
	void CreateEnvCubemapPrefilter();
	void Create2DLUTTexture();
};