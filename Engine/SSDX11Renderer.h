#pragma once

#include "Core.h"
#include "SSRenderer.h"


class ENGINE_API SSDX11Renderer : public SSRenderer
{
public:
	SSDX11Renderer();	

	virtual void Initialize(HWND windowHandle) override;
	virtual void Shutdown() override;
	virtual void OnWindowResize(int newWidth, int newHeight) override;
	virtual void DrawScene() override;
	
	inline ID3D11Device* GetDevice() const { return mDevice.Get(); }
	inline ID3D11DeviceContext* GetImmediateDeviceContext() const { return mDeviceContext.Get(); }
	inline ID3D11DeviceContext* GetDeferredDeviceContext() const { return mDeferredContext.Get(); }
	inline IDXGISwapChain* GetSwapChain() const { return mSwapChain.Get(); }

	void AppendRenderCommand(class SSDrawCmdBase* cmd);
	void FlushRenderCommands();

protected:
	void DrawCubeScene();
	void DrawSponzaScene();
	
	std::shared_ptr<class SSViewport> mViewport = nullptr;

	std::vector<class SSDrawCmdBase*> mRenderCommandList;

	void TestCompileShader();
	void TestCreateResources();

private:
	virtual bool CreateDevice() override;
	virtual bool CreateSwapChain() override;

	HWND mWindowHandle;
	UINT m4xMSAAQuality;

	bool bInitialized = false;

	bool bGbufferDump = false;

private:
	ComPtr<ID3D11Device> mDevice = nullptr;
	ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
	ComPtr<ID3D11DeviceContext> mDeferredContext = nullptr;
	ComPtr<IDXGISwapChain> mSwapChain = nullptr;

	ID3D11Debug* mDebug = nullptr;
	ID3D11SamplerState* mDefaultSamplerState = nullptr;

	std::shared_ptr<class SSMaterial> mTestMaterial;
	std::shared_ptr<class SSMaterial> mTBNDebugMaterial;
	std::shared_ptr<class SSMaterial> mDeferredLightMaterial;

	std::shared_ptr<class SSCube> mTestCube;
	std::shared_ptr<class SSRenderTargetCube> mRenderTargetCube;	

	std::vector<std::shared_ptr<class SSSPhere>> mSphereList;

	std::shared_ptr<class SSSphere> mTestSphere;
	std::shared_ptr<class SSSphere> mTestSphere2;

	std::shared_ptr<class SSScreenBlit> mScreenBlit;
	// 	

	std::shared_ptr<class SSVertexShader> mScreenBlitVertexShader;
	std::shared_ptr<class SSPixelShader> mScreenBlitPixelShader;

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

	std::shared_ptr<class SSPBRSphere> mRustedIron;
	std::shared_ptr<class SSPBRSphere> mTile;
	std::shared_ptr<class SSPBRSphere> mMetalGrid;


	// @ cubemap rendertargets
	std::shared_ptr<class SSCubemapRenderTarget> mEquirectToCubemapRenderTarget;
	std::shared_ptr<class SSCubemapRenderTarget> mConvolutionRenderTarget;
	std::shared_ptr<class SSPrefilterCubemapRenderTarget> mPrefilterRenderTarget;

	std::shared_ptr<class SSTextureCube> mEnvCubemap;
	std::shared_ptr<class SSTextureCube> mEnvCubemapConvolution;
	std::shared_ptr<class SSTextureCube> mEnvCubemapPrefilter;
	std::shared_ptr<class SSTexture2D> m2DLUTTexture;
	// @
	
	std::shared_ptr<class SSTexture2D> mHDREnvmap;

	std::shared_ptr<class SSGBuffer> mGBuffer;

	std::shared_ptr<class SSGenericRenderTarget> m2DLUTRenderTarget;
	//

	std::shared_ptr<class SSObjMesh> mSponzaMesh;
	std::shared_ptr<class SSObjMesh> mObjMeshSphere;

	std::shared_ptr<class SSPostProcess> mFXAAPostProcess;
	std::shared_ptr<class SSGBufferDumpPostProcess> mGBufferDumpProcess;
	std::shared_ptr<class SSPostProcess> mDeferredLightPostProcess;

	std::shared_ptr<class SSText3D> mText3D;

private:
	bool TryLoadEnvCubemap(std::wstring filepath);
	bool TryLoadEnvCubemapConvolution(std::wstring filepath);
	bool TryLoadEnvCubemapPrefilter(std::wstring filepath);
	bool TryLoad2DLUTTexture();

	void CreateEnvCubemap();
	void CreateEnvCubemapConvolution();
	void CreateEnvCubemapPrefilter();
	void Create2DLUTTexture();
	void Resize(int width, int height);
};