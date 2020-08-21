
#pragma once

#include <vector>
#include "SSDX11Buffer.h"

class SSIndexBuffer : public SSDX11Buffer
{
public:
    SSIndexBuffer(){}	
    void SetIndexBufferData(const std::vector<UINT> indexData, D3D11_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D11_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }
	UINT GetIndexCount()  { return mIndexCount; }
protected:
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT mIndexCount = 0;
};