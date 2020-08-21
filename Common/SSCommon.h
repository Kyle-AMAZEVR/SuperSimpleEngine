
#   ifdef GAMEMODULE_EXPORTS
#       define GAMEMODULE_API __declspec(dllexport)
#		define GAMEMODULE_API_EXTERN 
#   elif defined(GAMEMODULE_IMPORTS)
#       define GAMEMODULE_API __declspec(dllimport)
#		define GAMEMODULE_API_EXTERN extern
#   else
#       define GAMEMODULE_API
#   endif

#define STRINGFY(x) #x
#define TOSTRING(x) STRINGFY(x)
#define AT __FILE__ ":" TOSTRING(__LINE__)

void GAMEMODULE_API PrintError(const char* loc, const char* msg);

#define check(expression) if(!(expression)) { PrintError(AT, STRINGFY(expression));}

#define checkMsg(expression,msg) if(!(expression)) { PrintError(AT, STRINGFY(msg));}

#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3d11.h>
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

