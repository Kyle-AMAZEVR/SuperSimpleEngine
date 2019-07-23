
#   ifdef ENGINE_EXPORTS
#       define ENGINE_API __declspec(dllexport)
#   elif defined(ENGINE_IMPORTS)
#       define ENGINE_API __declspec(dllimport)
#   else
#       define ENGINE_API
#   endif

#define STB_IMAGE_IMPLEMENTATION 1

#include "d3d11.h"
//#include "dxgi.h"

#include "d3dcompiler.h"
#include "DirectXMath.h"
#include <cassert>
#include <string>

#define check(expression)\
if(!(expression))\
{\
    OutputDebugStringA("FAILED : ");\
    OutputDebugStringA(#expression);\
    OutputDebugStringA("\n");\
	__debugbreak();\
}

#define checkMsg(expression,msg)\
if(!(expression))\
{\
    OutputDebugStringA("FAILED : ");\
    OutputDebugStringA(#msg);\
    OutputDebugStringA("\n");\
	__debugbreak();\
}

//#if defined(DEBUG) | defined(_DEBUG)
#if 1
	#ifndef HR
    #define HR(x)									\
        {       									\
            HRESULT hr = (x);						\
            checkMsg(SUCCEEDED(hr),x)                    \
        }
    #endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif



#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }




