#pragma once

#include "Singleton.h"
#include "SSManagerBase.h"

bool operator<(const D3D11_DEPTH_STENCIL_DESC& a, const D3D11_DEPTH_STENCIL_DESC& b);

class SSDepthStencilStateManager : public Singleton<SSDepthStencilStateManager>, public SSManagerBase
{
public:	
	virtual void Initialize() override;
	virtual void Shutdown() override;

	// default
	void SetToDefault(ID3D11DeviceContext* deviceContext);

	// variation
	void SetDepthTestDisabled(ID3D11DeviceContext* deviceContext);
	void SetDepthCompLessEqual(ID3D11DeviceContext* deviceContext);

private:
	D3D11_DEPTH_STENCIL_DESC mDefaultDesc;

	D3D11_DEPTH_STENCIL_DESC mDepthTestDisabledDesc;	

	D3D11_DEPTH_STENCIL_DESC mDepthFuncLessEqualDesc;

	ID3D11DepthStencilState* mDefaultState = nullptr;	
	ID3D11DepthStencilState* mDepthTestDisabledState = nullptr;	
	ID3D11DepthStencilState* mDepthFuncLessEqualState = nullptr;
};


