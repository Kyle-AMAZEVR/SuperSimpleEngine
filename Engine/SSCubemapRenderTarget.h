#pragma once
#include "SSRenderTargetBase.h"
#include "SSTexture2D.h"
#include "SSTextureCube.h"

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



// @cubemap rt 
class SSCubemapRenderTarget : public IRenderTarget, public SSTextureCube
{
public:
	SSCubemapRenderTarget(UINT width, UINT height ,DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	virtual ~SSCubemapRenderTarget();
	
	void SetCurrentRTAs(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);

	virtual void SaveAsCubemapDDSFile(std::wstring filename);
	virtual void SaveFaceAsDDSFile(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);
	virtual void SaveFaceOfMipAsDDSFile(ID3D11DeviceContext* deviceContext, ECubemapFace eFace, UINT mip = 0);

	virtual void CreateCubemapShaderResource(ID3D11DeviceContext* deviceContext);

	void ClearFace(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);

	class SSRenderTargetTexture2D* GetRenderTargetTexture(ECubemapFace eFace);	

	// @IRenderTarget Interface
	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
	
	virtual void SetCurrentRenderTarget(ID3D11DeviceContext* deviceContext) override {}
	virtual void Clear(ID3D11DeviceContext* deviceContext) override;	
	virtual void Resize(UINT width, UINT height) override {}
	// @IRenderTarget Interface

protected:
	SSCubemapRenderTarget() {}

	class SSRenderTargetTexture2D* mRenderTargetArray[ECubemapFace::MAX]{ nullptr };

	virtual void InternalCreate();

	D3D11_VIEWPORT mViewport;	
	UINT mLastRTMip = 0;
};


// @ cubemap rt for prefiltering 
class SSPrefilterCubemapRenderTarget : public SSCubemapRenderTarget
{
public:
	SSPrefilterCubemapRenderTarget(UINT width, UINT height, UINT maxMipCount = 1, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	virtual void CreateCubemapShaderResource(ID3D11DeviceContext* deviceContext) override;	
	void SetCurrentRTAs(ID3D11DeviceContext* deviceContext, ECubemapFace eFace, UINT mip);
	
protected:
	virtual void InternalCreate() override;		
};