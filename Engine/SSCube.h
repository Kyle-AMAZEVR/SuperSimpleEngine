#pragma once

#include "SSSceneObject.h"

// Supser Simple
class ENGINE_API SSCube : public SSSceneObject
{
public:
	SSCube();
	
	virtual void Draw(ID3D11DeviceContext* deviceContext) override;


protected:
	void InternalCreate();
	static bool bIsInitialized;
	static std::shared_ptr<class SSVertexBuffer> mCubeVB;
	static std::shared_ptr<class SSIndexBuffer> mCubeIB;
};