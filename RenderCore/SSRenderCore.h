

#pragma once
#   ifdef RENDERCORE_EXPORTS
#       define RENDERCORE_API __declspec(dllexport)
#		define RENDERCORE_API_EXTERN extern __declspec(dllexport)
#   elif defined(RENDERCORE_IMPORTS)
#       define RENDERCORE_API __declspec(dllimport)
#		define RENDERCORE_API_EXTERN extern __declspec(dllimport)
#   else
#	ifndef RENDERCORE_API
#       define RENDERCORE_API
#	endif
#   endif
