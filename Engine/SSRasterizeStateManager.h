#pragma once

#include "Singleton.h"

class SSRaterizeStateManager : public Singleton<SSRaterizeStateManager>
{
public:
	void Initialize();
	void Shutdown();
	
	void SetToDefault();
	void SetCullModeNone();
	
	
private:
	D3D11_RASTERIZER_DESC mDefaultDesc;
	D3D11_RASTERIZER_DESC mCullModeNoneDesc;

	ID3D11RasterizerState* mDefaultState = nullptr;
	ID3D11RasterizerState* mCullModeNoneState = nullptr;
};