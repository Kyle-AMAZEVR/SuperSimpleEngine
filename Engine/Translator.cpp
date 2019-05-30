
#include "Core.h"

#include "Translator.h"


DXGI_FORMAT Translator::GetVertexShaderInputType(D3D11_SIGNATURE_PARAMETER_DESC desc)
{
    unsigned char nComponentCount = 0;
    unsigned char nMask = desc.Mask;

    while((nMask & 0x01) != 0)
    {
        nComponentCount++;
        nMask = nMask >> 1;        
    }

    if(desc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32)
    {
        switch(nComponentCount)
        {
            case 4:
                return DXGI_FORMAT_R32G32B32A32_FLOAT;
                break;
            case 3:
                return DXGI_FORMAT_R32G32B32_FLOAT;
                break;
            case 2:
                return DXGI_FORMAT_R32G32_FLOAT;
                break;
            case 1:
                return DXGI_FORMAT_R32_FLOAT;
                break;
        }                
    }

    return DXGI_FORMAT_R32_FLOAT;
}