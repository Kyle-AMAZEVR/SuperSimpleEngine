#pragma once

#include "Core.h"

class ENGINE_API SSDX11Device
{
public:

private:
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;
}