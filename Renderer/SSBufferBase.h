#pragma once

#include "SSRenderThreadObject.h"
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSBufferBase : public SSRenderThreadResidentObject
{
public:
    virtual UINT GetBufferIndex() { return mBufferIndex; }
    virtual UINT GetBufferSize() { return mBufferSize; }
    std::string GetBufferName() { return mBufferName; }

    virtual ID3D11Buffer* const* GetDX11BufferPointerRef() = 0;
    virtual ID3D11Buffer* GetDX11BufferPointer() = 0;	

protected:

    virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext) {}
    virtual void SubmitDataToDevice() {}

    virtual ~SSBufferBase();

    UINT mBufferIndex = 0;
    UINT mBufferSize = 0;

    std::string mBufferName;
};


