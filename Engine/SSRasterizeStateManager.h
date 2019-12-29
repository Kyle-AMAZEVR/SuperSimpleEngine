#pragma once

#include "SSManagerBase.h"
#include "Singleton.h"

class SSRasterizeStateManager : public Singleton<SSRasterizeStateManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;

	void SetToDefault(ID3D11DeviceContext* deviceContext);	
	void SetCullModeNone(ID3D11DeviceContext* deviceContext);
	void SetFrontCounterClockwise(ID3D11DeviceContext* deviceContext);
	void SetWireFrameMode(ID3D11DeviceContext* deviceContext);	
	
private:
	D3D11_RASTERIZER_DESC mDefaultDesc;
	D3D11_RASTERIZER_DESC mCullModeNoneDesc;
	D3D11_RASTERIZER_DESC mFrontCounterClockwiseDesc;
	D3D11_RASTERIZER_DESC mWireFrameDesc;

	ID3D11RasterizerState* mDefaultState = nullptr;
	ID3D11RasterizerState* mCullModeNoneState = nullptr;
	ID3D11RasterizerState* mFrontCounterClockwiseState = nullptr;
	ID3D11RasterizerState* mWireFrameState = nullptr;
};