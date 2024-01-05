#pragma once



#include "SSRenderThreadResidentObject.h"
#include <wrl/client.h>
#include <string>

using Microsoft::WRL::ComPtr;

class SSBufferBase
{
public:
    SSBufferBase();

    virtual UINT GetBufferIndex() { return mBufferSlotIndex; }
    virtual UINT GetBufferSize() { return mBufferSize; }
    std::string GetBufferName() { return mBufferName; }

    virtual BYTE* GetBufferDataPtr() = 0;
    virtual void* GetBufferPointer() = 0;
    virtual void* const* GetBufferPointerRef() = 0;
protected:
    virtual ~SSBufferBase();

    UINT mBufferSlotIndex = 0;
    UINT mBufferSize = 0;
    std::string mBufferName;
};


