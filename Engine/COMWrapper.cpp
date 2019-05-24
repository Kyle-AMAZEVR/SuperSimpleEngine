
#include "Core.h"
#include "COMWrapper.h"


COMWrapper::COMWrapper(IUnknown* object)
: mObject(object)
{
    mObject->AddRef();
}

COMWrapper::~COMWrapper()
{
    if(mObject)
    {
        mObject->Release();
        mObject = nullptr;
    }
}
