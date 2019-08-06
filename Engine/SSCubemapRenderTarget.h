#pragma once
#include "SSRenderTargetBase.h"
#include "SSTexture2D.h"

enum class ECubemapFace : unsigned char
{
	POSITIVE_X,
	NEGATIVE_X,

	POSITIVE_Y,
	NEGATIVE_Y,
	
	POSITIVE_Z,
	NEGATIVE_Z,

	MAX,
};


class SSCubemapRenderTarget : public IRenderTarget, public SSTexture2DBase
{
public:
	SSCubemapRenderTarget(UINT width, UINT height, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	void SetCurrentRTAs(ECubemapFace eFace);

	void SetCurrentRTAsPositiveX();
	void SetCurrentRTAsNegativeX();

	void SetCurrentRTAsPositiveY();
	void SetCurrentRTAsNegativeY();

	void SetCurrentRTAsPositiveZ();
	void SetCurrentRTAsNegativeZ();

	void CreateCubemapResource();

	void ClearFace(ECubemapFace eFace);

	virtual void Destroy() override;

	class SSRenderTargetTexture2D* GetRenderTargetTexture(ECubemapFace eFace);	

	// @IRenderTarget Interface
	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
	virtual void SetCurrentRenderTarget() override {}
	virtual void Clear() override;	
	virtual void Resize(UINT width, UINT height) override;
	// @IRenderTarget Interface

protected:

	class SSRenderTargetTexture2D* mRenderTargetArray[ECubemapFace::MAX]{ nullptr };

	void InternalCreate();

	D3D11_VIEWPORT mViewport;
};
