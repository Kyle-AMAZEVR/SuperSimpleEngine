
#   ifdef ENGINE_EXPORTS
#       define ENGINE_API __declspec(dllexport)
#   elif defined(ENGINE_IMPORTS)
#       define ENGINE_API __declspec(dllimport)
#   else
#       define ENGINE_API
#   endif

#include "d3d11.h"