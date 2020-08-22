#pragma once

#include "SSBufferBase.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class GAMEMODULE_API SSDX11Buffer : public SSBufferBase
{
public:
    void Destroy() override;

    ID3D11Buffer* const* GetDX11BufferPointerRef() override;
    ID3D11Buffer* GetDX11BufferPointer() override;

protected:

	D3D11_BUFFER_DESC mBufferDescription;
    ComPtr<ID3D11Buffer> mpBuffer = nullptr;
};


