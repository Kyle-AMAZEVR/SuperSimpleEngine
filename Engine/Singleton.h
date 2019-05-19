

#pragma once


template<class T>
class Singleton
{
public:
    T& Get() 
    { 
        if(mInstance == nullptr)
        {
            mInstance = new T();
        }
        return *mInstance; 
    }
private:
    T* mInstance = nullptr;
};