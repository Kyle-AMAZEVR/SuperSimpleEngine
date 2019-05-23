

#pragma once

class ENGINE_API COMWrapper
{
public : 
    COMWrapper(IUnknown* object);

protected:
    IUnknown* object = nullptr;
};