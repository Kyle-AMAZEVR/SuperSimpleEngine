#pragma once
#include "SSSceneObject.h"


class ENGINE_API SSScreenBlit : public SSSceneObject
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
protected:
	void InternalCreate();
	static bool bIsInitialized;
	static class SSVertexBuffer* mScreenVB;
	static class SSIndexBuffer* mScreenIB;
};