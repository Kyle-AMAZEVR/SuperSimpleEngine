#pragma once


#include "SSDX11RenderTarget.h"

class SSDX11Texture2D;
class SSDX11Device;

class SSDX11PostProcess
{
public:	
	SSDX11PostProcess(UINT width , UINT height, UINT rtCount=1, bool bDepthExist =false, bool bFixed =false, DXGI_FORMAT format = DXGI_FORMAT_R16G16B16A16_FLOAT);
	
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0) {}
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0, SSDX11Texture2D* input1) {}
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2){}
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2, SSDX11Texture2D* input3) {}
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2, SSDX11Texture2D* input3, SSDX11Texture2D* input4) {}
	virtual void Draw(SSDX11Device* deviceContext, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2, SSDX11Texture2D* input3, SSDX11Texture2D* input4, SSDX11Texture2D* input5 ) {}

	virtual void Destroy();

	SSDX11RenderTargetTexture2D* GetOutput(UINT nIndex);

	virtual void OnResize(UINT width, UINT height);

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }
	UINT GetRTCount() const { return mRTCount; }

protected:
	std::unique_ptr<class SSDX11RenderTarget> mRenderTarget;
	std::shared_ptr<class SSScreenBlit> mScreenBlit;

	UINT mWidth;
	UINT mHeight;
	bool mFixedSize = false;
	bool mDepthExist = false;
	UINT mRTCount = 1;
};
