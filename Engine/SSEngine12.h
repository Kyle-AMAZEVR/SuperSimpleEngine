#pragma once
#include "Singleton.h"
#include <wrl.h>
#include <wrl/client.h>
#include <d3d12.h>

using Microsoft::WRL::ComPtr;


class SSEngine12 : public Singleton<SSEngine12>
{
	
public:
	void Initialize(HWND windowHandle);

	void DrawScene();

protected:
	void LoadAssets();
	void WaitForPreviousFrame();
	void PopulateCommandList();

	static const UINT FrameCount = 3;

	bool CreateDevice();

	bool CreateSwapChain();
	ComPtr<IDXGIFactory4> mFactory;
	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12PipelineState> mPipelineState;
	ComPtr<IDXGISwapChain3> mSwapChain;
	ComPtr<ID3D12DescriptorHeap> mRTVHeap;
	ComPtr<ID3D12Resource> mRenderTargets[FrameCount];
	ComPtr<ID3D12RootSignature> mRootSignature;
	

	int mBufferWidth = 1024;
	int mBufferHeight = 768;
	
	HWND mWindowHandle;	
	UINT mRTVDescriptorSize = 0;

	UINT64 mFenceValue = 0;
	

	UINT mFrameIndex;

	ComPtr<ID3D12Fence> mFence;
	HANDLE mFenceEvent;

};
