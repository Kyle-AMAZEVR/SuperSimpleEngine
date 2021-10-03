
#   ifdef SSDX12_EXPORTS
#       define SSDX12_API __declspec(dllexport)
#   elif defined(SSDX12_IMPORTS)
#       define SSDX12_API __declspec(dllimport)
#   else
#       define SSDX12_API
#   endif

#define STB_IMAGE_IMPLEMENTATION 1

#include <d3d12.h>
#include <dxgi1_4.h>
#include <windows.h>
#include <wrl.h>
#include <wrl/client.h>
#include <combaseapi.h>
#include "SSGameModule.h"
#include "d3dcompiler.h"
#include "DirectXMath.h"
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <functional>
#include "d3dx12.h"

using Microsoft::WRL::ComPtr;
using namespace DirectX;

#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

void PrintDXError(ID3DBlob* errorMsg);

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


