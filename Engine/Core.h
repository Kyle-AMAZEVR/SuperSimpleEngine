
#   ifdef ENGINE_EXPORTS
#       define ENGINE_API __declspec(dllexport)
#   elif defined(ENGINE_IMPORTS)
#       define ENGINE_API __declspec(dllimport)
#   else
#       define ENGINE_API
#   endif

#include "d3d11.h"
//#include "dxgi.h"

#include "d3dcompiler.h"
//#include "DirectXMath.h"
#include <cassert>
#include <string>

#define check(expression)\
if(!(expression))\
{\
	__debugbreak();\
}

//#if defined(DEBUG) | defined(_DEBUG)
#if 1
	#ifndef HR
    #define HR(x)									\
        {       									\
            HRESULT hr = (x);						\
            check(SUCCEEDED(hr))                    \
        }
    #endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif



#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }




