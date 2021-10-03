
#include "SSDXTranslator.h"

UINT SSDXTranslator::GetDXGIFormatByteSize(DXGI_FORMAT format)
{
	switch(format)
	{
	case DXGI_FORMAT_R32G32B32A32_FLOAT:
		return sizeof(float) * 4;
	case DXGI_FORMAT_R32G32B32_FLOAT:
		return sizeof(float) * 3;
	case DXGI_FORMAT_R32G32_FLOAT:
		return sizeof(float) * 2;
	case DXGI_FORMAT_R32_FLOAT:
		return sizeof(float);

	case DXGI_FORMAT_R32G32B32A32_UINT:
		return sizeof(UINT) * 4;
	case DXGI_FORMAT_R32G32B32_UINT:
		return sizeof(UINT) * 3;
	case DXGI_FORMAT_R32G32_UINT:
		return sizeof(UINT) * 2;
	case DXGI_FORMAT_R32_UINT:
		return sizeof(UINT);

	case DXGI_FORMAT_R32G32B32A32_SINT:
		return sizeof(int) * 4;
	case DXGI_FORMAT_R32G32B32_SINT:
		return sizeof(int) * 3;
	case DXGI_FORMAT_R32G32_SINT:
		return sizeof(int) * 2;
	case DXGI_FORMAT_R32_SINT:
		return sizeof(int);
	}

	return 0;
}

DXGI_FORMAT SSDXTranslator::GetVertexShaderInputType(D3D12_SIGNATURE_PARAMETER_DESC desc)
{
	unsigned char nComponentCount = 0;
	unsigned char nMask = desc.Mask;

	while ((nMask & 0x01) != 0)
	{
		nComponentCount++;
		nMask = nMask >> 1;
	}

	return GetVertexShaderInputType(desc.ComponentType, nComponentCount);
}

DXGI_FORMAT SSDXTranslator::GetVertexShaderInputType(D3D_REGISTER_COMPONENT_TYPE type, const UINT componentCount)
{
	if (type == D3D_REGISTER_COMPONENT_FLOAT32)
	{
		switch (componentCount)
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

	else if (type == D3D_REGISTER_COMPONENT_UINT32)
	{
		switch (componentCount)
		{
		case 4:
			return DXGI_FORMAT_R32G32B32A32_UINT;
			break;
		case 3:
			return DXGI_FORMAT_R32G32B32_UINT;
			break;
		case 2:
			return DXGI_FORMAT_R32G32_UINT;
			break;
		case 1:
			return DXGI_FORMAT_R32_UINT;
			break;
		}
	}

	else if (type == D3D_REGISTER_COMPONENT_SINT32)
	{
		switch (componentCount)
		{
		case 4:
			return DXGI_FORMAT_R32G32B32A32_SINT;
			break;
		case 3:
			return DXGI_FORMAT_R32G32B32_SINT;
			break;
		case 2:
			return DXGI_FORMAT_R32G32_SINT;
			break;
		case 1:
			return DXGI_FORMAT_R32_SINT;
			break;
		}
	}

	return DXGI_FORMAT_R32_FLOAT;
}


DXGI_FORMAT SSDXTranslator::GetVertexShaderInputType(D3D11_SIGNATURE_PARAMETER_DESC desc)
{
    unsigned char nComponentCount = 0;
    unsigned char nMask = desc.Mask;

    while((nMask & 0x01) != 0)
    {
        nComponentCount++;
        nMask = nMask >> 1;        
    }

	return GetVertexShaderInputType(desc.ComponentType, nComponentCount);
}

