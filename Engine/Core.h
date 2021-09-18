
#   ifdef DX11RENDERER_EXPORTS
#       define THIS_IS_ENGINE_MODULE
#       define DX11RENDERER_API __declspec(dllexport)
#   elif defined(DX11RENDERER_IMPORTS)
#       define DX11RENDERER_API __declspec(dllimport)
#   else
#       define DX11RENDERER_API
#   endif


#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11_1.h>
#include <windows.h>
#include <wrl.h>
#include <combaseapi.h>
#include "../GameModule/SSCommon.h"
#include "d3dcompiler.h"
#include "DirectXMath.h"
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <functional>

#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)


#define check(expression) if(!(expression)) { PrintError(AT, STRINGFY(expression));}




#define checkMsg(expression,msg) if(!(expression)) { PrintError(AT, STRINGFY(msg));}

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
#define InitD3DDesc(x)  ZeroMemory(&x, sizeof(x)) 

using namespace DirectX;


