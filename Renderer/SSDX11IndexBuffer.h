
#pragma once

#include <vector>
#include "SSIndexBuffer.h"

class SSDX11IndexBuffer : public SSIndexBuffer
{
public:
    SSDX11IndexBuffer(){}

	virtual void SetIndexBufferData(const std::vector<UINT> indexData, 
		D3D_PRIMITIVE_TOPOLOGY ePrimitiveType = D3D_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	virtual void* GetBufferPointer() override;
	virtual void* const* GetBufferPointerRef() override;

protected:
	D3D11_BUFFER_DESC mBufferDescription;
	ComPtr<ID3D11Buffer> mpBuffer = nullptr;
};