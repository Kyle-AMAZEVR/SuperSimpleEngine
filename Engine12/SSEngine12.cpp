
#include "SSDX12.h"
#include "SSEngine12.h"
#include "SSVertexElementDeclaration.h"
#include <filesystem>
#include "SSMathHelper.h"
#include "SSTemplates.h"
#include "SSDX12ConstantBuffer.h"
#include "SSDX12Texture2D.h"


void SSDX12Engine::Initialize(HWND windowHandle)
{
	mWindowHandle = windowHandle;

	mAspectRatio = static_cast<float>(mBufferWidth) / static_cast<float>(mBufferHeight);

	mViewport.TopLeftX = mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mBufferWidth);
	mViewport.Height = static_cast<float>(mBufferHeight);

	mScissorRect.left = mScissorRect.top = 0;
	mScissorRect.right = mBufferWidth;
	mScissorRect.bottom = mBufferHeight;
	
	bool bDeviceCreated = CreateDevice();
	check(bDeviceCreated);

	//@ Create Command Queue , Command Allocator
	D3D12_COMMAND_QUEUE_DESC desc{};
	HR(mDevice->CreateCommandQueue(&desc, IID_PPV_ARGS(&mCommandQueue)));
	
	//@ Create swap chain
	CreateSwapChain();
	   
	HR(mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCommandAllocator)));
	
	HR(mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCommandAllocator.Get(), nullptr, IID_PPV_ARGS(&mCommandList)));	

	//@ save descriptor size
	mRTVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	mDSVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	mCBVSRVUAVDescriptorSize = mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//

	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	msQualityLevels.SampleCount = 1;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;

	HR(mDevice->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &msQualityLevels, sizeof(msQualityLevels)));

	m4xMSAAQuality = msQualityLevels.NumQualityLevels;

	mFenceValues = 0;

	HR(mDevice->CreateFence(mFenceValues, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence)));

	mFenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	if (mFenceEvent == nullptr)
	{
		check(false);
	}
	CreateDescriptorHeaps();

	LoadAssets();
	OnWindowResize(mBufferWidth, mBufferHeight);
		
	//mCommandList->Close();
	mCommandList->Reset(mCommandAllocator.Get(), nullptr);	
	
	CreateConstantBuffers();
	CreateTextures();
	CreateRootSignature();
	CreateBoxGeometry(mCommandList.Get());	

	HR(mCommandList->Close());

	ID3D12CommandList* cmdsLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	WaitForPreviousFrame();
}

void SSDX12Engine::CreateRootSignature()
{
	CD3DX12_ROOT_PARAMETER slotRootParameter[2];	

	CD3DX12_DESCRIPTOR_RANGE textureTable;
	textureTable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	slotRootParameter[0].InitAsDescriptorTable(1, &textureTable, D3D12_SHADER_VISIBILITY_PIXEL);
	slotRootParameter[1].InitAsConstantBufferView(0);

	auto staticSamplers = GetStaticSamplers();

	CD3DX12_ROOT_SIGNATURE_DESC rootSignatureDesc(sizeof_array(slotRootParameter), slotRootParameter, 
		staticSamplers.size(), staticSamplers.data(), 
		D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> serializeRootSig = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;

	HR(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &serializeRootSig, &errorBlob));

	if(errorBlob != nullptr)
	{
		check(false);
	}

	HR(mDevice->CreateRootSignature(0, serializeRootSig->GetBufferPointer(), serializeRootSig->GetBufferSize(), IID_PPV_ARGS(&mRootSignature)));
}

void SSDX12Engine::CreateConstantBuffers()
{
	mTestCBuffer = std::make_unique<SSDX12TypedConstantBuffer<ModelViewProjConstant>>(mDevice.Get());
}

void SSDX12Engine::CreateTextures()
{
	mTexture2D = std::make_unique<SSDX12Texture2D>();
	mTexture2D->LoadFromDDSFile(mDevice.Get(), mCommandList.Get(), L"./Resource/Tex/rustediron2_basecolor.dds");
}


UINT SSDX12Engine::CalcConstantBufferByteSize(UINT ByteSize)
{
	return (ByteSize + 255) & ~255;
}

void SSDX12Engine::CreateDescriptorHeaps()
{
	//@ create rtv descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HR(mDevice->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&mRTVHeap)));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());

	for (UINT n = 0; n < FrameCount; ++n)
	{
		HR(mSwapChain->GetBuffer(n, IID_PPV_ARGS(&mRenderTargets[n])));
		mDevice->CreateRenderTargetView(mRenderTargets[n].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, mRTVDescriptorSize);
	}

	//@ create dsv descriptor heap
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	HR(mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDSVHeap)));

	
}


void SSDX12Engine::OnWindowResize(int newWidth, int newHeight)
{
	check(mDevice != nullptr);
	check(mSwapChain != nullptr);

	mBufferWidth = newWidth;
	mBufferHeight = newHeight;

	WaitForPreviousFrame();

	HR(mCommandList->Close());
	HR(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));

	for(int i = 0; i < FrameCount; ++i)
	{
		mRenderTargets[i].Reset();
	}

	mDepthStencilBuffer.Reset();

	HR(mSwapChain->ResizeBuffers(FrameCount, mBufferWidth, mBufferHeight, DXGI_FORMAT_R16G16B16A16_FLOAT, DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH));

	mFrameIndex = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart());	
	for(int i = 0; i < FrameCount; ++i)
	{
		HR(mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mRenderTargets[i])));		
		mDevice->CreateRenderTargetView(mRenderTargets[i].Get(), nullptr, rtvHeapHandle);
		rtvHeapHandle.Offset(1, mRTVDescriptorSize);
	}

	//

	

	D3D12_RESOURCE_DESC depthStencilDesc;
	depthStencilDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthStencilDesc.Alignment = 0;
	depthStencilDesc.Width = mBufferWidth;
	depthStencilDesc.Height = mBufferHeight;
	depthStencilDesc.DepthOrArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.Format = DXGI_FORMAT_R24G8_TYPELESS;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = m4xMSAAQuality - 1;
	depthStencilDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	depthStencilDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_CLEAR_VALUE optClear;
	optClear.Format = mDepthStencilFormat;
	optClear.DepthStencil.Depth = 1.f;
	optClear.DepthStencil.Stencil = 0;

	HR(mDevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&depthStencilDesc,
		D3D12_RESOURCE_STATE_COMMON,
		&optClear, IID_PPV_ARGS(&mDepthStencilBuffer)));

	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc;
	dsvDesc.Flags = D3D12_DSV_FLAG_NONE;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	dsvDesc.Format = mDepthStencilFormat;
	dsvDesc.Texture2D.MipSlice = 0;

	CD3DX12_CPU_DESCRIPTOR_HANDLE handle(mDSVHeap->GetCPUDescriptorHandleForHeapStart());
	mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), &dsvDesc, handle);
	
	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mDepthStencilBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));

	HR(mCommandList->Close());

	ID3D12CommandList* cmdLists[] = { mCommandList.Get() };
	mCommandQueue->ExecuteCommandLists(_countof(cmdLists), cmdLists);

	
	WaitForPreviousFrame();

	mViewport.TopLeftX = 0;
	mViewport.TopLeftY = 0;
	mViewport.Width = static_cast<float>(mBufferWidth);
	mViewport.Height = static_cast<float>(mBufferHeight);

	mScissorRect.left = mScissorRect.top = 0;
	mScissorRect.right = mBufferWidth;
	mScissorRect.bottom = mBufferHeight;

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25 * XM_PI, mAspectRatio, 1.f, 1000.f);
	XMStoreFloat4x4(&mProj, P);
}

void SSDX12Engine::LoadAssets()
{
	CreateRootSignature();

	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;
	ComPtr<ID3DBlob> error;

	check(std::filesystem::exists(L"./Shader\\SimpleShader.vs"));

	HR(D3DCompileFromFile(L"./Shader\\Color.hlsl", nullptr, nullptr, "VS", "vs_5_0", 0, 0, &vertexShader, &error));
	HR(D3DCompileFromFile(L"./Shader\\Color.hlsl", nullptr, nullptr, "PS", "ps_5_0", 0, 0, &pixelShader, nullptr));

	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc{};

	psoDesc.InputLayout = { SSVertexElementDeclaration::PositionColorTexcoordElementDesc, 3 };
	psoDesc.pRootSignature = mRootSignature.Get();		
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
	psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	psoDesc.DepthStencilState.DepthEnable = false;
	psoDesc.DepthStencilState.StencilEnable = false;	
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R16G16B16A16_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	HR(mDevice->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&mPipelineState)));
}

void SSDX12Engine::Update()
{
	// Convert Spherical to Cartesian coordinates.
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	mTheta += (0.1f / XM_PI);

	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

	XMMATRIX world = SSMathHelper::IdentityMatrix4X4;
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view*proj;

	// Update the constant buffer with the latest worldViewProj matrix.
	ModelViewProjConstant objConstants;
	XMStoreFloat4x4(&objConstants.ModelViewProj, XMMatrixTranspose(worldViewProj));	

	if(mTestCBuffer)
	{
		mTestCBuffer->WriteData(objConstants);
	}
}

void SSDX12Engine::DrawScene()
{
	Update();

	PopulateCommandList();

	// Execute the command list.
	ID3D12CommandList* ppCommandLists[] = { mCommandList.Get() };
	
	mCommandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);

	// Present the frame.
	HR(mSwapChain->Present(1, 0));

	WaitForPreviousFrame();
}

bool SSDX12Engine::CreateSwapChain()
{	
	DXGI_SWAP_CHAIN_DESC1 sd{};	
	
	sd.BufferCount = FrameCount;
	sd.Width = mBufferWidth;
	sd.Height = mBufferHeight;	
	sd.Format = DXGI_FORMAT_R16G16B16A16_FLOAT;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.SampleDesc.Count = 1;	

	ComPtr<IDXGISwapChain1> tempSwapChain = nullptr;
	HR(mFactory->CreateSwapChainForHwnd(mCommandQueue.Get(), mWindowHandle, &sd, nullptr, nullptr, &tempSwapChain));
	tempSwapChain.As(&mSwapChain);	
	HR(mFactory->MakeWindowAssociation(mWindowHandle, DXGI_MWA_NO_ALT_ENTER));
	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

	return true;
}

void SSDX12Engine::PopulateCommandList()
{
	HR(mCommandAllocator->Reset());

	HR(mCommandList->Reset(mCommandAllocator.Get(), mPipelineState.Get()));
		
	mCommandList->RSSetViewports(1, &mViewport);
	mCommandList->RSSetScissorRects(1, &mScissorRect);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT,  D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(mRTVHeap->GetCPUDescriptorHandleForHeapStart(), mFrameIndex, mRTVDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(mDSVHeap->GetCPUDescriptorHandleForHeapStart());
	

	// Record commands.
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };

	mCommandList->OMSetRenderTargets(1, &rtvHandle, true, &dsvHandle);
	mCommandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	mCommandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1, 0, 0, nullptr);	
	
	ID3D12DescriptorHeap* descriptorHeaps[]{ mTexture2D->GetDescriptorHeap().Get() };
	mCommandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	mCommandList->SetGraphicsRootSignature(mRootSignature.Get());

	mCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	mCommandList->IASetVertexBuffers(0, 1, &mTestVertexBuffer->GetVertexBufferView());
	mCommandList->IASetIndexBuffer(&mTestIndexBuffer->GetIndexBufferView());
	

	mCommandList->SetGraphicsRootDescriptorTable(0, mTexture2D->GetDescriptorHeap()->GetGPUDescriptorHandleForHeapStart());
	mCommandList->SetGraphicsRootConstantBufferView(1, mTestCBuffer->GetResource()->GetGPUVirtualAddress());

	mCommandList->DrawIndexedInstanced(mTestIndexBuffer->GetIndexCount(), 1, 0, 0, 0);

	mCommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(mRenderTargets[mFrameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));
	
	mCommandList->Close();
}


bool SSDX12Engine::CreateDevice()
{	
	ComPtr<IDXGIAdapter1> hardwareAdapter;

	UINT dxgiFactoryFlags = 0;

#if defined(_DEBUG)
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();

			// Enable additional debug layers.
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	
	HR(CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&mFactory)));
	IDXGIAdapter1* adapter;

	int adapterIndex = 0;
	bool adapterFound = false;

	while(mFactory->EnumAdapters1(adapterIndex, &adapter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if(desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			adapterIndex++;
			continue;
		}

		HRESULT hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), &mDevice);
		if(SUCCEEDED(hr))
		{
			adapterFound = true;
			break;
		}

		adapterIndex++;
	}

	return adapterFound;
}

void SSDX12Engine::WaitForPreviousFrame()
{	
	const UINT64 currentFenceValue = mFenceValues;
	HR(mCommandQueue->Signal(mFence.Get(), currentFenceValue));

	auto CompletedValue = mFence->GetCompletedValue();

	mFrameIndex = mSwapChain->GetCurrentBackBufferIndex();

	// If the next frame is not ready to be rendered yet, wait until it is ready.
	if (CompletedValue < mFenceValues)
	{
		HR(mFence->SetEventOnCompletion(mFenceValues, mFenceEvent));
		WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);
	}	

	// Set the fence value for the next frame.
	mFenceValues = currentFenceValue + 1;	

}


void SSDX12Engine::WaitForGPU()
{
	// Schedule a Signal command in the queue.
	HR(mCommandQueue->Signal(mFence.Get(), mFenceValues));

	// Wait until the fence has been processed.
	HR(mFence->SetEventOnCompletion(mFenceValues, mFenceEvent));
	WaitForSingleObjectEx(mFenceEvent, INFINITE, FALSE);

	// Increment the fence value for the current frame.
	mFenceValues++;
}

void SSDX12Engine::CreateBoxGeometry(ID3D12GraphicsCommandList* CmdList)
{
	std::vector<Vertex> vertices =
	{
		Vertex({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1,1,1,1), XMFLOAT2(0,0) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(0,0,0,1), XMFLOAT2(0,1) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(1,0,0,1), XMFLOAT2(1,1) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(0,1,0,1), XMFLOAT2(1,0) }),
		Vertex({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(0,0,1,1), XMFLOAT2(0,0) }),
		Vertex({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(1,1,0,1), XMFLOAT2(0,1) }),
		Vertex({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(1,0,1,1), XMFLOAT2(1,1) }),
		Vertex({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(0,1,1,1), XMFLOAT2(1,0) })
	};

	std::vector<UINT> indices =
	{
		// front face
		0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7
	};		

	mTestVertexBuffer = std::make_unique<SSDX12TypedVertexBuffer<Vertex>>(mDevice.Get(), CmdList, vertices);
	mTestIndexBuffer = std::make_unique<SSDX12IndexBuffer>(mDevice.Get(), CmdList, indices);	
}


ComPtr<ID3D12Resource> SSDX12Engine::CreateDefaultBuffer(ID3D12GraphicsCommandList* CmdList, const void* InitialData, const UINT64 ByteSize, ComPtr<ID3D12Resource>& UploadBuffer)
{
	ComPtr<ID3D12Resource> DefaultBuffer;
	
	HR(mDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE, 
		&CD3DX12_RESOURCE_DESC::Buffer(ByteSize),
		D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(&DefaultBuffer)));


	HR(mDevice->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(ByteSize),
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&UploadBuffer)));


	D3D12_SUBRESOURCE_DATA subResourceData{};
	subResourceData.pData = InitialData;
	subResourceData.RowPitch = ByteSize;
	subResourceData.SlicePitch = ByteSize;

	CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));

	UpdateSubresources<1>(CmdList, DefaultBuffer.Get(), UploadBuffer.Get(), 0, 0, 1, &subResourceData);

	CmdList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DefaultBuffer.Get(),
		D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));

	return DefaultBuffer;
}


std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> SSDX12Engine::GetStaticSamplers()
{
	// Applications usually only need a handful of samplers.  So just define them all up front
	// and keep them available as part of the root signature.  

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };
}

