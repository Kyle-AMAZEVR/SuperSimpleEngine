


#   ifdef COMMON_EXPORTS
#       define COMMON_API __declspec(dllexport)
#   elif defined(COMMON_IMPORTS)
#       define COMMON_API __declspec(dllimport)
#   else
#       define COMMON_API
#   endif


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
