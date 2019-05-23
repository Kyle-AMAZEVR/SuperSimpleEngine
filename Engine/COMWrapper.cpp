
#include "Core.h"
#include "COMWrapper.h"


COMWrapper::COMWrapper(IUnknown* object)
{
    object->AddRef();
}

