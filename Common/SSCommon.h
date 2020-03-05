
#   ifdef COMMON_EXPORTS
#       define COMMON_API __declspec(dllexport)
#		define COMMON_API_EXTERN 
#   elif defined(COMMON_IMPORTS)
#       define COMMON_API __declspec(dllimport)
#		define COMMON_API_EXTERN extern
#   else
#       define COMMON_API
#   endif

#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

void PrintError(const char* loc, const char* msg);

#define check(expression) if(!(expression)) { PrintError(AT, STRINGFY(expression));}

#define checkMsg(expression,msg) if(!(expression)) { PrintError(AT, STRINGFY(msg));}

#include <d3d12.h>
#include <dxgi1_4.h>
#include "d3d11.h"
#include <windows.h>
#include <wrl.h>
#include <combaseapi.h>
#include "d3dcompiler.h"
#include "DirectXMath.h"
#include <cassert>
#include <string>
#include <map>
#include <vector>
#include <functional>

