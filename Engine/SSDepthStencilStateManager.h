#pragma once

#include "Singleton.h"

bool operator<(const D3D11_DEPTH_STENCIL_DESC& a, const D3D11_DEPTH_STENCIL_DESC& b);

class SSDepthStencilStateManager : public Singleton<SSDepthStencilStateManager>
{
public:	
	void Initialize();
	void Shutdown();

	// default
	void SetToDefault();

	// variation
	void SetDepthTestDisabled();
	void SetDepthCompLessEqual();

private:
	D3D11_DEPTH_STENCIL_DESC mDefaultDesc;

	D3D11_DEPTH_STENCIL_DESC mDepthTestDisabledDesc;	

	D3D11_DEPTH_STENCIL_DESC mDepthFuncLessEqualDesc;

	ID3D11DepthStencilState* mDefaultState = nullptr;	
	ID3D11DepthStencilState* mDepthTestDisabledState = nullptr;	
	ID3D11DepthStencilState* mDepthFuncLessEqualState = nullptr;
};


