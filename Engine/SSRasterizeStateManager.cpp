
#include "Core.h"
#include "SSRasterizeStateManager.h"
#include "SSEngine.h"

void SSRaterizeStateManager::Initialize()
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

	HR(SSEngine::Get().GetDevice()->CreateRasterizerState(&mDefaultDesc, &mDefaultState));
	
	mCullModeNoneDesc = mDefaultDesc;
	mCullModeNoneDesc.CullMode = D3D11_CULL_NONE;

	HR(SSEngine::Get().GetDevice()->CreateRasterizerState(&mCullModeNoneDesc, &mCullModeNoneState));


	mFrontCounterClockwiseDesc = mDefaultDesc;
	mFrontCounterClockwiseDesc.FrontCounterClockwise = true;

	HR(SSEngine::Get().GetDevice()->CreateRasterizerState(&mFrontCounterClockwiseDesc, &mFrontCounterClockwiseState));

	mWireFrameDesc = mDefaultDesc;
	mWireFrameDesc.FillMode = D3D11_FILL_WIREFRAME;

	HR(SSEngine::Get().GetDevice()->CreateRasterizerState(&mWireFrameDesc, &mWireFrameState));

}

void SSRaterizeStateManager::SetToDefault()
{
	SSEngine::Get().GetImmediateDeviceContext()->RSSetState(mDefaultState);
}
void SSRaterizeStateManager::SetCullModeNone()
{
	SSEngine::Get().GetImmediateDeviceContext()->RSSetState(mCullModeNoneState);
}

void SSRaterizeStateManager::SetFrontCounterClockwise()
{
	SSEngine::Get().GetImmediateDeviceContext()->RSSetState(mFrontCounterClockwiseState);
}

void SSRaterizeStateManager::SetWireFrameMode()
{
	SSEngine::Get().GetImmediateDeviceContext()->RSSetState(mWireFrameState);
}

void SSRaterizeStateManager::Shutdown()
{
	ReleaseCOM(mCullModeNoneState);
	ReleaseCOM(mDefaultState);
	ReleaseCOM(mFrontCounterClockwiseState);
	ReleaseCOM(mWireFrameState);
}