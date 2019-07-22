#pragma once

#include "DXRenderResource.h"

class SSBufferBase : public DXRenderResource
{
public:	
	virtual void Destroy() override;

    ID3D11Buffer*& GetBufferPointerRef() { return mpBuffer; }
    ID3D11Buffer* GetBufferPointer() { return mpBuffer;}
    virtual UINT GetBufferIndex() { return mBufferIndex;}
    virtual UINT GetBufferSize() { return mBufferSize; }
    std::string GetBufferName() { return mBufferName; }

protected:

    virtual void SubmitDataToDevice() {}

    virtual ~SSBufferBase();
    D3D11_BUFFER_DESC mBufferDescription;
    ID3D11Buffer* mpBuffer = nullptr;
    
    UINT mBufferIndex = 0;
    UINT mBufferSize = 0;

    std::string mBufferName;
};


