

#pragma once

#include "Singleton.h"
#include "SSManagerBase.h"

class ENGINE_API SSSamplerManager : public Singleton<SSSamplerManager>, public SSManagerBase
{
public:
	virtual void Initialize() override;
	virtual void Shutdown() override;
	
	ID3D11SamplerState* GetDefaultSamplerState() { return mDefaultSamplerState; }
	
protected:
	void CreateDefaultSampler();
	ID3D11SamplerState* mDefaultSamplerState = nullptr;
};
