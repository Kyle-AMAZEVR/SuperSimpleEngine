#pragma once

#include "SSBufferBase.h"
#include "d3d11.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSDX11Buffer : public SSBufferBase
{
public:   
    ID3D11Buffer* const* GetDX11BufferPointerRef();
    ID3D11Buffer* GetDX11BufferPointer();
    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext){}

protected:
	D3D11_BUFFER_DESC mBufferDescription;
    ComPtr<ID3D11Buffer> mpBuffer = nullptr;
};


