#pragma once

#include <d3d11.h>

class ENGINE_API IDrawable
{
public:
	virtual void Draw(ID3D11DeviceContext* deviceContext){}
};
