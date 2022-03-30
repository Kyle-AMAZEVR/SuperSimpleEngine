#pragma once
#include <d3dcommon.h>
#include "SSBufferBase.h"
#include <vector>

class RENDERCORE_API SSIndexBufferBase : public SSBufferBase
{
public:
	virtual void SetIndexBufferData(const std::vector<UINT> indexData, D3D_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST) = 0;
	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }
	UINT GetIndexCount() { return mIndexCount; }
protected:
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	UINT mIndexCount = 0;
};
