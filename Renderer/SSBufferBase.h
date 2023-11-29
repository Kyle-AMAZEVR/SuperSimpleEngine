#pragma once


#include "SSRenderCore.h"
#include "SSRenderThreadResidentObject.h"
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;

class DX11RENDERER_API SSBufferBase : public SSRenderThreadResidentObject
{
public:
    SSBufferBase();

    virtual UINT GetBufferIndex() { return mBufferIndex; }
    virtual UINT GetBufferSize() { return mBufferSize; }
    std::string GetBufferName() { return mBufferName; }
    virtual void* GetBufferPointer() = 0;
    virtual void* const* GetBufferPointerRef() = 0;
protected:
    virtual ~SSBufferBase();

    UINT mBufferIndex = 0;
    UINT mBufferSize = 0;

    std::string mBufferName;
};


