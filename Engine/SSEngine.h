#pragma once

#   ifdef ENGINE_EXPORTS
#		define THIS_IS_ENGINE_MODULE
#       define ENGINE_API __declspec(dllexport)
#		define ENGINE_API_EXTERN extern __declspec(dllexport)
#   elif defined(ENGINE_IMPORTS)
#       define ENGINE_API __declspec(dllimport)
#		define ENGINE_API_EXTERN extern __declspec(dllimport)
#   else
#       define ENGINE_API
#   endif
