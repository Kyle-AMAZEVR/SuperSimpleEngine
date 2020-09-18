
#include "Core.h"
#include "SSRasterizeStateManager.h"
#include "SSDX11Engine.h"

void SSRasterizeStateManager::Initialize()
{
	InitD3DDesc(mDefaultDesc);
	
	// https://docs.microsoft.com/en-us/windows/win32/api/d3d11/ns-d3d11-d3d11_rasterizer_desc	
	mDefaultDesc.FillMode = D3D11_FILL_SOLID;
	mDefaultDesc.CullMode = D3D11_CULL_BACK;
	mDefaultDesc.FrontCounterClockwise = false;
	mDefaultDesc.DepthBias = 0;
	mDefaultDesc.SlopeScaledDepthBias = 0;
	mDefaultDesc.DepthBiasClamp = 0;
	mDefaultDesc.DepthClipEnable = true;
	mDefaultDesc.ScissorEnable = false;
	mDefaultDesc.MultisampleEnable = false;
	mDefaultDesc.AntialiasedLineEnable = false;

	
	HR(SSDX11Engine::Get().GetDevice()->CreateRasterizerState(&mDefaultDesc, &mDefaultState));
	
	mCullModeNoneDesc = mDefaultDesc;
	mCullModeNoneDesc.CullMode = D3D11_CULL_NONE;

	HR(SSDX11Engine::Get().GetDevice()->CreateRasterizerState(&mCullModeNoneDesc, &mCullModeNoneState));


	mFrontCounterClockwiseDesc = mDefaultDesc;
	mFrontCounterClockwiseDesc.FrontCounterClockwise = true;

	HR(SSDX11Engine::Get().GetDevice()->CreateRasterizerState(&mFrontCounterClockwiseDesc, &mFrontCounterClockwiseState));

	mWireFrameDesc = mDefaultDesc;
	mWireFrameDesc.FillMode = D3D11_FILL_WIREFRAME;

	HR(SSDX11Engine::Get().GetDevice()->CreateRasterizerState(&mWireFrameDesc, &mWireFrameState));

}

void SSRasterizeStateManager::SetToDefault(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetState(mDefaultState);
}

void SSRasterizeStateManager::SetCullModeNone(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetState(mCullModeNoneState);
}

void SSRasterizeStateManager::SetFrontCounterClockwise(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetState(mFrontCounterClockwiseState);
}

void SSRasterizeStateManager::SetWireFrameMode(ID3D11DeviceContext* deviceContext)
{
	deviceContext->RSSetState(mWireFrameState);
}

void SSRasterizeStateManager::Shutdown()
{
	ReleaseCOM(mCullModeNoneState);
	ReleaseCOM(mDefaultState);
	ReleaseCOM(mFrontCounterClockwiseState);
	ReleaseCOM(mWireFrameState);
}