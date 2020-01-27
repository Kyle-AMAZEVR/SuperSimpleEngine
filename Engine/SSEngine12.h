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

	void LoadPipeline();

	ComPtr<ID3D12Device> mDevice;
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12PipelineState> mPipelineState;
	bool mUseWarpDevice = false;

	void GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter);
};
