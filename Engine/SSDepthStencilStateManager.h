#pragma once

#include "Singleton.h"

bool operator<(const D3D11_DEPTH_STENCIL_DESC& a, const D3D11_DEPTH_STENCIL_DESC& b)
{
	if (a.DepthEnable < b.DepthEnable) return true;
	if (a.DepthEnable > b.DepthEnable) return false;

	if (a.StencilEnable > b.StencilEnable) return true;
	if (a.StencilEnable < b.StencilEnable) return false;

	if (a.DepthFunc > b.DepthFunc) return true;
	if (a.DepthFunc < b.DepthFunc) return false;
	
	return false;
}

class SSDepthStencilStateManager : Singleton<SSDepthStencilStateManager>
{
public:	
	void Initialize();
	void Shutdown();

	// default
	void SetToDefault();

	// variation
	void SetDepthTestDisabled();
	void SetDepthCompLessEqual();



protected:
	D3D11_DEPTH_STENCIL_DESC mDefaultDesc;

	D3D11_DEPTH_STENCIL_DESC mDepthTestDisabledDesc;	

	D3D11_DEPTH_STENCIL_DESC mDepthFuncLessEqualDesc;

	ID3D11DepthStencilState* mDefaultState = nullptr;	
	ID3D11DepthStencilState* mDepthTestDisabledState = nullptr;	
	ID3D11DepthStencilState* mDepthFuncLessEqualState = nullptr;
};


