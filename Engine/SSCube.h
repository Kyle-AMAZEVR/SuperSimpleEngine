#pragma once

#include "DXRenderResource.h"

// Supser Simple
class ENGINE_API SSCube : public DXRenderResource
{
public:
	SSCube();

protected:
	void InternalCreate();

	static std::unique_ptr<class DXVertexBuffer> mCubeVB;
	static std::unique_ptr<class DXIndexBuffer> mCubeIB;
};