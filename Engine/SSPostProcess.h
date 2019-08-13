#pragma once


class ENGINE_API SSPostProcess : public DXRenderResource
{
public:	
	SSPostProcess(UINT width , UINT height, UINT rtCount=1, bool bDepthExist =false, bool bFixed =false);
	virtual ~SSPostProcess();

	virtual void Draw(SSTexture2DBase* input0) {}
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1) {}
	virtual void OnResize(UINT width, UINT height){}

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }

protected:
	std::unique_ptr<class SSGenericRenderTarget> mRenderTarget;
	UINT mWidth;
	UINT mHeight;
	bool mFixedSize = false;
	bool mDepthExist = false;
	UINT mRTCount = 1;
};

class ENGINE_API SSFXAAPostProcess : public SSPostProcess
{
public:

	virtual void Destroy() override;
};