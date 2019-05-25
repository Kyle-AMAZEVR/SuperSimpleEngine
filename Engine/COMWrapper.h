

#pragma once

template<class T>
class ENGINE_API COMWrapper
{
public:
    COMWrapper(T* object);
    ~COMWrapper();
    inline T* Get() { return mObject; }
protected:
    T* mObject = nullptr;
};

template<class T>
COMWrapper<T>::COMWrapper(T* object)
: mObject(object)
{
}

template<class T>
COMWrapper<T>::~COMWrapper()
{
    if(mObject)
    {
        auto refCount = mObject->Release();

        if(refCount == 0)
        {
            //
        }

        mObject = nullptr;
    }
}
