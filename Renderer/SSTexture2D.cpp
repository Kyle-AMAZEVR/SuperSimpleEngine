


#include "SSRendererModulePCH.h"
#include "SSTexture2D.h"
#include "SSDX11Renderer.h"

SSTexture2D::SSTexture2D()
{	
}

bool SSTexture2D::Release()
{
	ReleaseCOM(mTexturePtr);
	ReleaseCOM(mShaderResourceView);

	return true;
}

