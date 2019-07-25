
#pragma once

class ENGINE_API DXRenderResource
{
public:
	DXRenderResource();
	virtual ~DXRenderResource();
	virtual void Destroy(){}
};



UPROPERTY(Replicated)
int32 HP;