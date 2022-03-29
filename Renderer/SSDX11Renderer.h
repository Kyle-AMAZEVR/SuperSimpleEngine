#pragma once

#include "SSDX11Device.h"
#include "SSRendererModulePCH.h"
#include "SSRenderer.h"

class DX11RENDERER_API SSDX11Renderer : public SSRenderer
{
public:
	SSDX11Renderer();	

	virtual void Initialize(HWND windowHandle) override;
	virtual void Shutdown() override;
	virtual void OnWindowResize(int newWidth, int newHeight) override;
	virtual void DrawScene() override;

	
	static SSDX11Renderer& Get();
	static SSDX11Renderer* GetPtr();
	

	void AppendRenderCommand(class SSDrawCmdBase* cmd);
	void FlushRenderCommands();

	DXGI_FORMAT SwapChainFormat = DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetImmediateDeviceContext();

protected:
	void DrawCubeScene();
	void DrawSponzaScene();
	void DrawSkybox();
	void DrawDummyScene();
	
	std::shared_ptr<class SSViewport> mViewport = nullptr;

	std::vector<class SSDrawCmdBase*> mRenderCommandList;

	static SSDX11Renderer* mRendererInstance;

	void TestCompileShader();
	void TestCreateResources();

private:		
	bool bInitialized = false;
	bool bGbufferDump = false;

private:
	SSDX11Device* mDX11Device;	

	ID3D11SamplerState* mDefaultSamplerState = nullptr;

	std::vector<SSAdapterInfo> mAdapterInfos;

	std::shared_ptr<class SSMaterial> mTestMaterial;
	std::shared_ptr<class SSMaterial> mTBNDebugMaterial;
	std::shared_ptr<class SSMaterial> mDeferredLightMaterial;

	std::shared_ptr<class SSRenderTargetCube> mRenderTargetCube;
	std::vector<std::shared_ptr<class SSSPhere>> mSphereList;

	std::shared_ptr<class SSCubeMapRenderingSphere> mCubemapSphere;

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

	std::shared_ptr<class SSDX11GBuffer> mGBuffer;

	std::shared_ptr<class SSDX11RenderTarget> m2DLUTRenderTarget;
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