#pragma once
#include "SSDX12.h"

#include <wrl/client.h>
#include <d3d12.h>
#include "d3dx12.h"
#include "SSEngineBase.h"

using Microsoft::WRL::ComPtr;


class SSDX12_API SSDX12Engine : public SSEngineBase
{
	
public:
	virtual void Initialize(HWND windowHandle) override;
	virtual void DrawScene() override;
	virtual void OnWindowResize(int newWidth, int newHeight) override;
	virtual void Shutdown() override {}

protected:
	void CalcDescriptorHeapSize();
	void CreateDescriptorHeaps();
	void LoadAssets();
	void WaitForPreviousFrame();
	void MoveToNextFrame();
	void WaitForGPU();
	void PopulateCommandList();
	void CreateRootSignature();
	void CreateConstantBuffers();

	ComPtr<ID3D12Resource> CreateDefaultBuffer(ID3D12GraphicsCommandList* CmdList, const void* InitialData, const UINT64 ByteSize, ComPtr<ID3D12Resource>& UploadBuffer);

	static const UINT FrameCount = 3;

	virtual bool CreateDevice() override;

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
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView;
	

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
};
