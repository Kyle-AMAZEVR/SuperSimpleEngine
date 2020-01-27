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


protected:

	bool CreateDevice();

	bool CreateSwapChain();

	void LoadPipeline();

	ComPtr<IDXGIFactory4> mFactory;
	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12PipelineState> mPipelineState;
	ComPtr<IDXGISwapChain3> mSwapChain;
	ComPtr<ID3D12DescriptorHeap> mRTVHeap;

	
	bool mUseWarpDevice = false;
	HWND mWindowHandle;

	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);

	UINT mWidth = 1024;
	UINT mHeight = 768;
	UINT mRTVDescriptorSize = 0;

	static const UINT FrameCount = 2;

	UINT mFrameIndex;

	ComPtr<ID3D12Fence> mFence;

	HANDLE mFenceEvent;
};
