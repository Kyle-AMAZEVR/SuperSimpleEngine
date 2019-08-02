
#include "Core.h"
#include "SSDepthStencilStateManager.h"
#include "SSEngine.h"


void SSDepthStencilStateManager::Initialize()
{
	InitD3DDesc(mDefaultDesc);
	InitD3DDesc(mDepthTestDisabledDesc);

	//https://docs.microsoft.com/ko-kr/windows/win32/api/d3d11/ns-d3d11-d3d11_depth_stencil_desc
	mDefaultDesc.DepthEnable = true;
	mDefaultDesc.StencilEnable = false;
	mDefaultDesc.DepthFunc = D3D11_COMPARISON_LESS;
	mDefaultDesc.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	mDefaultDesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	mDefaultDesc.FrontFace.StencilFunc = mDefaultDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	mDefaultDesc.FrontFace.StencilDepthFailOp = mDefaultDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	mDefaultDesc.FrontFace.StencilPassOp = mDefaultDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	mDefaultDesc.FrontFace.StencilFailOp = mDefaultDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;

	HR(SSEngine::Get().GetDevice()->CreateDepthStencilState(&mDefaultDesc, &mDefaultState));

	// depth disable
	mDepthTestDisabledDesc = mDefaultDesc;
	mDepthTestDisabledDesc.DepthEnable = false;
	HR(SSEngine::Get().GetDevice()->CreateDepthStencilState(&mDepthTestDisabledDesc, &mDepthTestDisabledState));

	// depth func less equal
	// for cubemap
	mDepthFuncLessEqualDesc = mDefaultDesc;
	mDepthFuncLessEqualDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;	
	HR(SSEngine::Get().GetDevice()->CreateDepthStencilState(&mDepthFuncLessEqualDesc, &mDepthFuncLessEqualState));	

}

void SSDepthStencilStateManager::SetToDefault()
{
	SSEngine::Get().GetDeviceContext()->OMSetDepthStencilState(mDefaultState, 1);
}

void SSDepthStencilStateManager::SetDepthTestDisabled()
{
	SSEngine::Get().GetDeviceContext()->OMSetDepthStencilState(mDepthTestDisabledState, 1);
}

void SSDepthStencilStateManager::SetDepthCompLessEqual()
{
	SSEngine::Get().GetDeviceContext()->OMSetDepthStencilState(mDepthFuncLessEqualState, 1);
}

void SSDepthStencilStateManager::Shutdown()
{
	ReleaseCOM(mDefaultState);
	ReleaseCOM(mDepthFuncLessEqualState);
	ReleaseCOM(mDepthTestDisabledState);
}