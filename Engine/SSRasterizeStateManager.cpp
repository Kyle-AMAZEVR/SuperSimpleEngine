
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
}

void SSRaterizeStateManager::SetToDefault()
{
	SSEngine::Get().GetDeviceContext()->RSSetState(mDefaultState);
}
void SSRaterizeStateManager::SetCullModeNone()
{
	SSEngine::Get().GetDeviceContext()->RSSetState(mCullModeNoneState);
}