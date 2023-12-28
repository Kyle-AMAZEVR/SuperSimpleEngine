#pragma once
#include "SSRenderTargetBase.h"
#include "SSTextureCube.h"

enum ECubemapFace
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
// create 6 2d rendertargets +x/-x, +y/-y, +z/-z

class SSCubemapRenderTarget : public IRenderTarget, public SSTextureCube
{
public:
	SSCubemapRenderTarget(UINT width, UINT height ,DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	virtual ~SSCubemapRenderTarget();
	
	void SetCurrentRTAs(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);

	virtual void SaveAsCubemapDDSFile(std::wstring filename);
	virtual void SaveFaceAsDDSFile(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);
	virtual void SaveFaceOfMipAsDDSFile(ID3D11DeviceContext* deviceContext, ECubemapFace eFace, UINT mip = 0);

	virtual void Copy2DRenderTargetToCubemap(ID3D11DeviceContext* deviceContext);

	void ClearFace(ID3D11DeviceContext* deviceContext, ECubemapFace eFace);

	// @IRenderTarget Interface
	virtual UINT GetRenderTargetWidth() const override { return mWidth; }
	virtual UINT GetRenderTargetHeight() const override { return mHeight; }
		
	void Clear(ID3D11DeviceContext* deviceContext);	
	virtual void Resize(UINT width, UINT height) override {}
	// @IRenderTarget Interface

protected:
	SSCubemapRenderTarget() {}

	class SSDX11RenderTargetTexture2D* mRenderTargetArray[ECubemapFace::MAX]{ nullptr };

	virtual void InternalCreate();

	D3D11_VIEWPORT mViewport;	
	UINT mLastRTMip = 0;
};


// @ cubemap rt for prefiltering 
class SSPrefilterCubemapRenderTarget : public SSCubemapRenderTarget
{
public:
	SSPrefilterCubemapRenderTarget(UINT width, UINT height, UINT maxMipCount = 1, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	virtual void Copy2DRenderTargetToCubemap(ID3D11DeviceContext* deviceContext) override;	
	void SetCurrentRTAs(ID3D11DeviceContext* deviceContext, ECubemapFace eFace, UINT mip);
	
protected:
	virtual void InternalCreate() override;		
};