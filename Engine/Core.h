
#   ifdef ENGINE_EXPORTS
#       define ENGINE_API __declspec(dllexport)
#   elif defined(ENGINE_IMPORTS)
#       define ENGINE_API __declspec(dllimport)
#   else
#       define ENGINE_API
#   endif

#include <d3d11.h>
#include <dxgi.h>
#include <cassert>


//#if defined(DEBUG) | defined(_DEBUG)
#if 1
	#ifndef HR
    #define HR(x)									\
        {       									\
            HRESULT hr = (x);						\
            if(FAILED(hr))							\
            {       								\
                LPWSTR output;                                    	\
                FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |		\
                    FORMAT_MESSAGE_IGNORE_INSERTS 	 |		\
                    FORMAT_MESSAGE_ALLOCATE_BUFFER,			\
                    NULL,						\
                    hr,						\
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),	\
                    (LPTSTR) &output,				\
                    0,						\
                    NULL);					        \
                MessageBoxW(NULL, output, L"Error", MB_OK);		\
            }								                    \
        }
    #endif
#else
	#ifndef HR
	#define HR(x) (x)
	#endif
#endif 

#define ReleaseCOM(x) { if(x){ x->Release(); x = nullptr; } }




