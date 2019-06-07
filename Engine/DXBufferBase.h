#pragma once

#include "DXRenderResource.h"

class DXBufferBase : public DXRenderResource
{
public:
    ID3D11Buffer*& GetBufferPointerRef() { return mpBuffer; }
    ID3D11Buffer* GetBufferPointer() { return mpBuffer;}
protected:
    virtual ~DXBufferBase();
    D3D11_BUFFER_DESC mBufferDescription;
    ID3D11Buffer* mpBuffer = nullptr;
    std::string mBufferName;
};


