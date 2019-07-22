#pragma once

#include "DXRenderResource.h"

// Supser Simple
class ENGINE_API SSCube : public DXRenderResource
{
public:
	SSCube();



protected:
	void InternalCreate();
	static bool bIsInitialized;
	static std::shared_ptr<class SSVertexBuffer> mCubeVB;
	static std::shared_ptr<class SSIndexBuffer> mCubeIB;
};