#pragma once

class DXBufferBase
{
protected:
    D3D11_BUFFER_DESC mBufferDescription;
};



class DXConstantBuffer : public DXBufferBase
{
public:
    DXConstantBuffer();
};