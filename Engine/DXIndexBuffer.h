
#pragma once

#include <vector>
#include "DXBufferBase.h"

class DXIndexBuffer : public DXBufferBase
{
public:
    DXIndexBuffer();

    void SetIndexBufferData(const std::vector<unsigned int> indexData);
protected:
    
};