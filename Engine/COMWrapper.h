

#pragma once

class ENGINE_API COMWrapper
{
public:
    COMWrapper(IUnknown* object);
    ~COMWrapper();
protected:
    IUnknown* mObject = nullptr;
};