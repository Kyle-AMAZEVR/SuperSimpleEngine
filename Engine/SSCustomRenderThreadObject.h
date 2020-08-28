#pragma once



#include "SSRenderThreadObject.h"
#include "SSName.h"

class SSCustomRenderThreadObject : public SSRenderThreadObject
{
public:
	SSCustomRenderThreadObject();

	void SetVertexShader(SSName name);
	void SetPixelShader(SSName name);

protected:
	std::shared_ptr<class SSVertexShader> mVS;
	std::shared_ptr<class SSPixelShader> mPS;
	
};
