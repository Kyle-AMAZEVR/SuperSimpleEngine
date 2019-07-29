#pragma once

class ENGINE_API SSRenderTargetBase : public SSTexture2DBase
{
public:
	virtual void MakeCurrentRenderTarget();
};