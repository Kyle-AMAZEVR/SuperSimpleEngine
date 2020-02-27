#pragma once
#include "SSDX12.h"

#include <wrl/client.h>
#include <d3d12.h>
#include "d3dx12.h"
#include "SSEngineBase.h"
#include "SSUploadBuffer.h"
#include "SSMeshGeometry.h"
#include "SSDX12ConstantBuffer.h"


struct ModelViewProjConstant
{
	XMFLOAT4X4 ModelViewProj;
};


class SSDX12_API SSDX12Engine : public SSEngineBase
{
	
public:
	virtual void Initialize(HWND windowHandle) override;
	virtual void DrawScene() override;
	virtual void OnWindowResize(int newWidth, int newHeight) override;
	virtual void Shutdown() override {}

	static UINT CalcConstantBufferByteSize(UINT ByteSize);

protected:	
	
	void LoadAssets();
	
	void WaitForPreviousFrame();	
	void WaitForGPU();
	void PopulateCommandList();

	virtual bool CreateDevice() override;
	void CreateDescriptorHeaps();
	void CreateRootSignature();
	void CreateConstantBuffers();
	void CreateBoxGeometry(ID3D12GraphicsCommandList* CmdList);
	void Update();

	ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12GraphicsCommandList* CmdList, const void* InitialData, const UINT64 ByteSize, ComPtr<ID3D12Resource>& UploadBuffer);

	static const UINT FrameCount = 3;

	XMFLOAT4X4 mProj;
	XMFLOAT4X4 mView;
	XMFLOAT4X4 mModel;


	bool CreateSwapChain();
	ComPtr<IDXGIFactory4> mFactory;
	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12PipelineState> mPipelineState;
	ComPtr<IDXGISwapChain3> mSwapChain;
	
	ComPtr<ID3D12DescriptorHeap> mRTVHeap;
	ComPtr<ID3D12DescriptorHeap> mDSVHeap;
	ComPtr<ID3D12DescriptorHeap> mCBVHeap;

	ComPtr<ID3D12Resource> mRenderTargets[FrameCount];
	ComPtr<ID3D12Resource> mDepthStencilBuffer;

	ComPtr<ID3D12RootSignature> mRootSignature;

	ComPtr<ID3D12Resource> mVertexBuffer;
	
	

	float mAspectRatio = 1.f;	

	UINT mRTVDescriptorSize = 0;
	UINT mDSVDescriptorSize = 0;
	UINT mCBVSRVUAVDescriptorSize = 0;
	
	UINT64 mFenceValues{ 0 };
	
	CD3DX12_VIEWPORT mViewport;
	CD3DX12_RECT mScissorRect;	

	UINT mFrameIndex;

	ComPtr<ID3D12Fence> mFence;
	HANDLE mFenceEvent;

	DXGI_FORMAT mBackbufferFormat = DXGI_FORMAT_R16G16B16A16_FLOAT;
	DXGI_FORMAT mDepthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	std::unique_ptr<class SSUploadBuffer<ModelViewProjConstant>> mMVPUploadBuffer;
	std::unique_ptr<class SSMeshGeometry> mMeshGeom = nullptr;

	std::unique_ptr<class SSDX12TypedConstantBuffer<ModelViewProjConstant>> mTestCBuffer;

	

	float mTheta = 1.5f*XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;
};
