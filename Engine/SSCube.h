#pragma once

#include "SSSceneObject.h"

// Supser Simple
class ENGINE_API SSCube : public SSSceneObject
{
public:
	SSCube();

	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	class SSVertexBuffer* GetVB() { return mCubeVB; }
	class SSIndexBuffer* GetIB() { return mCubeIB; }

protected:
	void InternalCreate();
	static bool bIsInitialized;
	static class SSVertexBuffer* mCubeVB;
	static class SSIndexBuffer* mCubeIB;
};