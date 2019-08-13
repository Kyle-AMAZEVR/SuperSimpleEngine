
#include "Core.h"
#include "DXVertexTypes.h"


UINT GetHashCode(const VT_PositionNormalTexcoordTangent& p)
{
	UINT x = static_cast<UINT>(p.VertexAttribute1.x * 100.f);
	UINT y = static_cast<UINT>(p.VertexAttribute1.y * 100.f);
	UINT z = static_cast<UINT>(p.VertexAttribute1.z * 100.f);

	return (3 * x + 5 * y + 7 * z) % (1 << 16);
}