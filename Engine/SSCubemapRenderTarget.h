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
	SSCubemapRenderTarget(UINT width, UINT height ,DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	void SetCurrentRTAs(ECubemapFace eFace);

	void SaveFaceAsDDSFile(ECubemapFace eFace);

	virtual void CreateCubemapShaderResource();	

	void ClearFace(ECubemapFace eFace);

	virtual void Destroy() override;

	class SSRenderTargetTexture2D* GetRenderTargetTexture(ECubemapFace eFace);	

	// @IRenderTarget Interface
	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
	virtual void SetCurrentRenderTarget() override {}
	virtual void Clear() override;	
	virtual void Resize(UINT width, UINT height) override {}
	// @IRenderTarget Interface

protected:
	SSCubemapRenderTarget() {}

	class SSRenderTargetTexture2D* mRenderTargetArray[ECubemapFace::MAX]{ nullptr };

	virtual void InternalCreate();

	D3D11_VIEWPORT mViewport;
	
	UINT mMipLevels = 0;
	UINT mLastRTMip = 0;
};


// @ cubemap rt for prefiltering 
class SSPrefilterCubemapRenderTarget : public SSCubemapRenderTarget
{
public:
	SSPrefilterCubemapRenderTarget(UINT width, UINT height, UINT maxMipCount = 1, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);	
	void SaveFaceOfMipAsDDSFile(ECubemapFace eFace, UINT mip = 0);
	virtual void CreateCubemapShaderResource() override;
	void SetCurrentRTAs(ECubemapFace eFace, UINT mip);
	
protected:
	
	virtual void InternalCreate() override;
	UINT mMipLevels = 0;
	UINT mLastRTMip = 0;
};