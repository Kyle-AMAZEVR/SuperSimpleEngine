
#   ifdef GAMEMODULE_EXPORTS
#		define THIS_IS_GAMEMODULE
#       define GAMEMODULE_API __declspec(dllexport)
#		define GAMEMODULE_API_EXTERN extern __declspec(dllexport)
#   elif defined(GAMEMODULE_IMPORTS)
#       define GAMEMODULE_API __declspec(dllimport)
#		define GAMEMODULE_API_EXTERN extern __declspec(dllimport)
#   else
#       define GAMEMODULE_API
#   endif

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

