#pragma once
#include "SSGameObject.h"


class ENGINE_API SSScreenBlit : public SSGameObject
{
public:
	SSScreenBlit();
	virtual void Draw(ID3D11DeviceContext* deviceContext);
	virtual void Destroy() override;
protected:
	void InternalCreate();
	static bool bIsInitialized;
	static class SSDX11VertexBuffer* mScreenVB;
	static class SSIndexBuffer* mScreenIB;
};