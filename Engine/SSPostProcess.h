#pragma once

#include "DXRenderResource.h"
#include "SSRenderTarget2D.h"

class SSTexture2DBase;

class ENGINE_API SSPostProcess : public DXRenderResource
{
public:	
	SSPostProcess(UINT width , UINT height, UINT rtCount=1, bool bDepthExist =false, bool bFixed =false, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	virtual void Draw(class SSTexture2DBase* input0) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2){}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2, SSTexture2DBase* input3) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2, SSTexture2DBase* input3, SSTexture2DBase* input4) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2, SSTexture2DBase* input3, SSTexture2DBase* input4, SSTexture2DBase* input5 ) {}

	virtual void Destroy() override;

	SSRenderTargetTexture2D* GetOutput(UINT nIndex);

	virtual void OnResize(UINT width, UINT height);

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }
	UINT GetRTCount() const { return mRTCount; }

protected:
	std::unique_ptr<class SSGenericRenderTarget> mRenderTarget;
	std::shared_ptr<class SSScreenBlit> mScreenBlit;

	UINT mWidth;
	UINT mHeight;
	bool mFixedSize = false;
	bool mDepthExist = false;
	UINT mRTCount = 1;
};
