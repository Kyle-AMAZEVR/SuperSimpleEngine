#pragma once

#include "DXRenderResource.h"

class DXBufferBase : public DXRenderResource
{
protected:
    virtual ~DXBufferBase();
    D3D11_BUFFER_DESC mBufferDescription;
    ID3D11Buffer* mpBuffer = nullptr;
};


