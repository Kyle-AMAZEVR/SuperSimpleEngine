#pragma once

#include <d3d11.h>
#include "SSRenderCore.h"

class SSVertexBuffer;
class SSIndexBuffer;

class RENDERCORE_API SSRenderDevice
{
public:
	virtual bool				InitializeDevice(HWND windowHandle) = 0;
	virtual bool				CreateDevice() = 0;

	virtual SSVertexBuffer*		CreateVertexBuffer() = 0;
	virtual SSIndexBuffer*		CreateIndexBuffer() = 0;

	virtual void				SetVertexShader(class SSVertexShader* vs) = 0;
	virtual void				SetPixelShader(class SSPixelShader* ps) = 0;

	virtual void				SetVSConstantBufferData() = 0;
	virtual void				SetPSConstantBufferData() = 0;
	
	virtual void				SetCurrentRenderTarget(class SSViewport* viewport) = 0;
	
	
	virtual void				ClearCurrentRenderTarget() = 0;

	virtual void				SetDepthTestDisabled() = 0;
	virtual void				SetDepthCompLessEqual() = 0;

	virtual void				SetRasterizerStateToDefault() = 0;
	virtual void				SetCullModeNone()=0;
	virtual void				SetFrontCounterClockwise()=0;
	virtual void				SetWireFrameMode()=0;

	virtual void				ResizeViewport(unsigned int width, unsigned int height) = 0;
	virtual void				ClearViewport() = 0;
	
protected:
	int mBufferWidth = 1920;
	int mBufferHeight = 1080;
	HWND mWindowHandle;
	UINT m4xMSAAQuality = 0;
};



