

#pragma once

#include "Singleton.h"

class ENGINE_API SSSamplerManager : public Singleton<SSSamplerManager>
{
public:
	void Initialize();

	ID3D11SamplerState* GetDefaultSamplerState() { return mDefaultSamplerState; }

protected:
	void CreateDefaultSampler();
	ID3D11SamplerState* mDefaultSamplerState = nullptr;
};