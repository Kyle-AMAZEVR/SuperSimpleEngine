#pragma once

#include "SSGameObject.h"

// Supser Simple
class ENGINE_API SSCube : public SSGameObject
{
public:
	SSCube();

	virtual void Draw(SSDX11Renderer* renderer) override;	
	virtual void Draw(ID3D11DeviceContext* deviceContext) override;
	class SSDX11VertexBuffer* GetVB() { return mCubeVB; }
	class SSIndexBuffer* GetIB() { return mCubeIB; }

	void PrepareRendering() override;

protected:	
	
	void InternalCreate();
	static bool bIsInitialized;
	static class SSDX11VertexBuffer* mCubeVB;
	static class SSIndexBuffer* mCubeIB;
};