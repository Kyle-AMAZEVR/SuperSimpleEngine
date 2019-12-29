#pragma once

#include "DXRenderResource.h"
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

class SSBufferBase : public DXRenderResource
{
public:	
	virtual void Destroy() override;

    ID3D11Buffer* const* GetBufferPointerRef() { return mpBuffer.GetAddressOf(); }
    ID3D11Buffer* GetBufferPointer() { return mpBuffer.Get();}
    virtual UINT GetBufferIndex() { return mBufferIndex;}
    virtual UINT GetBufferSize() { return mBufferSize; }
    std::string GetBufferName() { return mBufferName; }

protected:

	virtual void SubmitDataToDevice(ID3D11DeviceContext* deviceContext){}
    virtual void SubmitDataToDevice() {}

    virtual ~SSBufferBase();
    D3D11_BUFFER_DESC mBufferDescription;
	ComPtr<ID3D11Buffer> mpBuffer = nullptr;
    
    UINT mBufferIndex = 0;
    UINT mBufferSize = 0;

    std::string mBufferName;
};


