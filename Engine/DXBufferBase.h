#pragma once

#include "DXRenderResource.h"

class DXBufferBase : public DXRenderResource
{
protected:
    D3D11_BUFFER_DESC mBufferDescription;
    
};



class DXConstantBuffer : public DXBufferBase
{
public:
    DXConstantBuffer();
};