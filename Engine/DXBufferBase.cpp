
#include "Core.h"
#include "DXBufferBase.h"
#include "DXEngine.h"


DXBufferBase::~DXBufferBase()
{
    if(mpBuffer != nullptr)
    {
        mpBuffer->Release();
        mpBuffer = nullptr;
    }
}
