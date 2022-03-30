#pragma once

#include "SSBufferBase.h"
#include "d3d11.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11Buffer : public SSBufferBase
{
public:   
    virtual void* const* GetBufferPointerRef() override;
    virtual void* GetBufferPointer() override;
    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext){}
protected:

	D3D11_BUFFER_DESC mBufferDescription;
    ComPtr<ID3D11Buffer> mpBuffer = nullptr;
};


